/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.notifications

import chat.progressive.app.core.platform.ProgressiveViewModelAction

sealed interface ProgressiveSettingsNotifAction : ProgressiveViewModelAction {
    data class EnableNotificationsForDevice(val pushDistributor: String) : ProgressiveSettingsNotifAction
    object DisableNotificationsForDevice : ProgressiveSettingsNotifAction
    data class RegisterPushDistributor(val pushDistributor: String) : ProgressiveSettingsNotifAction
}
