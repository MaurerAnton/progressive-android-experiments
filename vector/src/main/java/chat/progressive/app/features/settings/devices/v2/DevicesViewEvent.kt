/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.devices.v2

import chat.progressive.app.core.platform.ProgressiveViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse
import org.matrix.android.sdk.api.session.crypto.model.CryptoDeviceInfo

sealed class DevicesViewEvent : ProgressiveViewEvents {
    data class RequestReAuth(
            val registrationFlowResponse: RegistrationFlowResponse,
            val lastErrorCode: String?
    ) : DevicesViewEvent()

    data class ShowVerifyDevice(val userId: String, val transactionId: String?) : DevicesViewEvent()
    object SelfVerification : DevicesViewEvent()
    data class ShowManuallyVerify(val cryptoDeviceInfo: CryptoDeviceInfo) : DevicesViewEvent()
    object PromptResetSecrets : DevicesViewEvent()
    object SignoutSuccess : DevicesViewEvent()
    data class SignoutError(val error: Throwable) : DevicesViewEvent()
}
