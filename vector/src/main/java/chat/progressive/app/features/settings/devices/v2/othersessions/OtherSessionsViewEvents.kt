/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.devices.v2.othersessions

import chat.progressive.app.core.platform.ProgressiveViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse

sealed class OtherSessionsViewEvents : ProgressiveViewEvents {
    data class RequestReAuth(
            val registrationFlowResponse: RegistrationFlowResponse,
            val lastErrorCode: String?
    ) : OtherSessionsViewEvents()

    object SignoutSuccess : OtherSessionsViewEvents()
    data class SignoutError(val error: Throwable) : OtherSessionsViewEvents()
}
