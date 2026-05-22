/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.crosssigning

import chat.progressive.app.core.platform.ProgressiveViewEvents
import org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse

/**
 * Transient events for cross signing settings screen.
 */
sealed class CrossSigningSettingsViewEvents : ProgressiveViewEvents {
    data class Failure(val throwable: Throwable) : CrossSigningSettingsViewEvents()
    data class RequestReAuth(val registrationFlowResponse: RegistrationFlowResponse, val lastErrorCode: String?) : CrossSigningSettingsViewEvents()
    data class ShowModalWaitingView(val status: String?) : CrossSigningSettingsViewEvents()
    object HideModalWaitingView : CrossSigningSettingsViewEvents()
}
