/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.settings.devices.v2.overview

import im.vector.app.core.platform.VectorViewModelAction

sealed class SessionOverviewAction : VectorViewModelAction {

    object VerifySession : SessionOverviewAction()
    object SignoutOtherSession : SessionOverviewAction()
    object SsoAuthDone : SessionOverviewAction()
    data class PasswordAuthDone(val password: String) : SessionOverviewAction()
    object ReAuthCancelled : SessionOverviewAction()
    data class TogglePushNotifications(
            val deviceId: String,
            val enabled: Boolean,
    ) : SessionOverviewAction()
    object ToggleIpAddressVisibility : SessionOverviewAction()
}
