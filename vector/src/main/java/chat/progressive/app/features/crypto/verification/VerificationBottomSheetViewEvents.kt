/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.crypto.verification

import chat.progressive.app.core.platform.ProgressiveViewEvents

/**
 * Transient events for the verification bottom sheet.
 */
sealed class VerificationBottomSheetViewEvents : ProgressiveViewEvents {
    object Dismiss : VerificationBottomSheetViewEvents()
    object DismissAndOpenDeviceSettings : VerificationBottomSheetViewEvents()
    object AccessSecretStore : VerificationBottomSheetViewEvents()
    object ResetAll : VerificationBottomSheetViewEvents()
    object GoToSettings : VerificationBottomSheetViewEvents()
    data class ConfirmCancel(val otherUserId: String, val deviceId: String?) : VerificationBottomSheetViewEvents()
    data class ModalError(val errorMessage: CharSequence) : VerificationBottomSheetViewEvents()
    data class RequestNotFound(val transactionId: String) : VerificationBottomSheetViewEvents()
}
