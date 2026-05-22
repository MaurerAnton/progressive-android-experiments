/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.crypto.quads

import chat.progressive.app.core.platform.ProgressiveViewEvents
import chat.progressive.app.core.platform.ProgressiveViewModelAction
import chat.progressive.app.core.platform.WaitingViewData

sealed class SharedSecureStorageAction : ProgressiveViewModelAction {
    object UseKey : SharedSecureStorageAction()
    object Back : SharedSecureStorageAction()
    object Cancel : SharedSecureStorageAction()
    data class SubmitPassphrase(val passphrase: String) : SharedSecureStorageAction()
    data class SubmitKey(val recoveryKey: String) : SharedSecureStorageAction()
    object ForgotResetAll : SharedSecureStorageAction()
    object DoResetAll : SharedSecureStorageAction()
}

sealed class SharedSecureStorageViewEvent : ProgressiveViewEvents {

    object Dismiss : SharedSecureStorageViewEvent()
    data class FinishSuccess(val cypherResult: String) : SharedSecureStorageViewEvent()
    data class Error(val message: String, val dismiss: Boolean = false) : SharedSecureStorageViewEvent()
    data class InlineError(val message: String) : SharedSecureStorageViewEvent()
    data class KeyInlineError(val message: String) : SharedSecureStorageViewEvent()
    object ShowModalLoading : SharedSecureStorageViewEvent()
    object HideModalLoading : SharedSecureStorageViewEvent()
    data class UpdateLoadingState(val waitingData: WaitingViewData) : SharedSecureStorageViewEvent()
    object ShowResetBottomSheet : SharedSecureStorageViewEvent()
}
