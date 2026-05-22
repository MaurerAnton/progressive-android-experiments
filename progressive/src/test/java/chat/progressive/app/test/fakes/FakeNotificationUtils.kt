/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.test.fakes

import android.app.Notification
import chat.progressive.app.features.notifications.InviteNotifiableEvent
import chat.progressive.app.features.notifications.NotificationUtils
import chat.progressive.app.features.notifications.SimpleNotifiableEvent
import io.mockk.every
import io.mockk.mockk

class FakeNotificationUtils {

    val instance = mockk<NotificationUtils>()

    fun givenBuildRoomInvitationNotificationFor(event: InviteNotifiableEvent, myUserId: String): Notification {
        val mockNotification = mockk<Notification>()
        every { instance.buildRoomInvitationNotification(event, myUserId) } returns mockNotification
        return mockNotification
    }

    fun givenBuildSimpleInvitationNotificationFor(event: SimpleNotifiableEvent, myUserId: String): Notification {
        val mockNotification = mockk<Notification>()
        every { instance.buildSimpleEventNotification(event, myUserId) } returns mockNotification
        return mockNotification
    }
}
