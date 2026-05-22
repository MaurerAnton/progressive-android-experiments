/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.notifications

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed interface ProgressiveSettingsNotifEvent : ProgressiveViewEvents {
    object NotificationsForDeviceEnabled : ProgressiveSettingsNotifEvent
    object NotificationsForDeviceDisabled : ProgressiveSettingsNotifEvent
    object AskUserForPushDistributor : ProgressiveSettingsNotifEvent
    object NotificationMethodChanged : ProgressiveSettingsNotifEvent
}
