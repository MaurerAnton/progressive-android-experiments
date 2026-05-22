/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.pin.lockscreen.ui

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class LockScreenViewEvent : ProgressiveViewEvents {
    data class ClearPinCode(val confirmationFailed: Boolean) : LockScreenViewEvent()
    object CodeCreationComplete : LockScreenViewEvent()
    data class AuthSuccessful(val method: AuthMethod) : LockScreenViewEvent()
    data class AuthFailure(val method: AuthMethod) : LockScreenViewEvent()
    data class AuthError(val method: AuthMethod, val throwable: Throwable) : LockScreenViewEvent()
    object ShowBiometricKeyInvalidatedMessage : LockScreenViewEvent()
    object ShowBiometricPromptAutomatically : LockScreenViewEvent()
}
