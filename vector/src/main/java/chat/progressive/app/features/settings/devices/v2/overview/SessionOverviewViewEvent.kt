/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.devices.v2.overview

import chat.progressive.app.core.platform.ProgressiveViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse

sealed class SessionOverviewViewEvent : ProgressiveViewEvents {
    object ShowVerifyCurrentSession : SessionOverviewViewEvent()
    data class ShowVerifyOtherSession(val deviceId: String) : SessionOverviewViewEvent()
    object PromptResetSecrets : SessionOverviewViewEvent()
    data class RequestReAuth(
            val registrationFlowResponse: RegistrationFlowResponse,
            val lastErrorCode: String?
    ) : SessionOverviewViewEvent()

    object SignoutSuccess : SessionOverviewViewEvent()
    data class SignoutError(val error: Throwable) : SessionOverviewViewEvent()
}
