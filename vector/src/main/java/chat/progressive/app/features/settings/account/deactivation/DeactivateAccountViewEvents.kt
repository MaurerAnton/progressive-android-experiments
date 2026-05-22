/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.account.deactivation

import chat.progressive.app.core.platform.ProgressiveViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse

/**
 * Transient events for deactivate account settings screen.
 */
sealed class DeactivateAccountViewEvents : ProgressiveViewEvents {
    data class Loading(val message: CharSequence? = null) : DeactivateAccountViewEvents()
    object InvalidAuth : DeactivateAccountViewEvents()
    data class OtherFailure(val throwable: Throwable) : DeactivateAccountViewEvents()
    object Done : DeactivateAccountViewEvents()
    data class RequestReAuth(val registrationFlowResponse: RegistrationFlowResponse, val lastErrorCode: String?) : DeactivateAccountViewEvents()
}
