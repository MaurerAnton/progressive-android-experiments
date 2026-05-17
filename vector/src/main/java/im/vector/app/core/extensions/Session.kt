/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.core.extensions

import android.content.Context
import androidx.core.content.ContextCompat
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.ProcessLifecycleOwner
import im.vector.app.core.services.VectorSyncAndroidService
import chat.progressive.app.native.ProgressiveNative
import im.vector.app.features.session.VectorSessionStore
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.crypto.keysbackup.KeysBackupState
import org.matrix.android.sdk.api.session.crypto.model.MXEventDecryptionResult
import org.matrix.android.sdk.api.session.events.model.Event
// import org.matrix.android.sdk.internal.crypto.EventDecryptor
// import org.matrix.android.sdk.internal.crypto.RustCryptoService
// import org.matrix.android.sdk.internal.session.room.timeline.TokenChunkEventPersistor
// import org.matrix.android.sdk.internal.session.sync.handler.room.RoomSyncHandler
import timber.log.Timber

fun Session.startSyncing(context: Context) {
    val applicationContext = context.applicationContext

    // Progressive Chat: native C++ Megolm decryptor (Labs-gated via nativeDecryptAttempt)
    // EventDecryptor.nativeDecryptAttempt = { event ->

    // Progressive Chat: create native OlmAccount (device identity) alongside Rust SDK
    try {
        ProgressiveNative.ensureLoaded()
        val deviceId = sessionParams.deviceId ?: "NATIVE01"
        val ok = ProgressiveNative.nativeOlmCreateAccount(myUserId, deviceId)
        if (ok) {
            val keys = ProgressiveNative.nativeOlmGetIdentityKeys()
            Timber.i("PROGRESSIVE native OlmAccount created: $myUserId/$deviceId keys=$keys")
            // Generate OTKs for key exchange
            val otks = ProgressiveNative.nativeOlmGenerateOneTimeKeys(50)
            Timber.d("PROGRESSIVE generated OTKs: len=${otks.length}")
        }
    } catch (e: Exception) {
        Timber.w(e, "PROGRESSIVE native OlmAccount creation skipped")
    }

    if (!syncService().hasAlreadySynced()) {
        // initial sync is done as a service so it can continue below app lifecycle
        VectorSyncAndroidService.newOneShotIntent(
                context = applicationContext,
                sessionId = sessionId
        )
                .let {
                    try {
                        ContextCompat.startForegroundService(applicationContext, it)
                    } catch (ex: Throwable) {
                        // TODO
                        Timber.e(ex)
                    }
                }
    } else {
        val isAtLeastStarted = ProcessLifecycleOwner.get().lifecycle.currentState.isAtLeast(Lifecycle.State.STARTED)
        Timber.v("--> is at least started? $isAtLeastStarted")
        syncService().startSync(isAtLeastStarted)
    }
}

/**
 * Tell is the session has unsaved e2e keys in the backup.
 */
suspend fun Session.hasUnsavedKeys(): Boolean {
    return cryptoService().inboundGroupSessionsCount(false) > 0 &&
            cryptoService().keysBackupService().getState() != KeysBackupState.ReadyToBackUp
}

suspend fun Session.cannotLogoutSafely(): Boolean {
    // has some encrypted chat
    return hasUnsavedKeys() ||
            // has local cross signing keys
            (cryptoService().crossSigningService().allPrivateKeysKnown() &&
                    // That are not backed up
                    !sharedSecretStorageService().isRecoverySetup())
}

fun Session.vectorStore(context: Context) = VectorSessionStore(context, myUserId)
