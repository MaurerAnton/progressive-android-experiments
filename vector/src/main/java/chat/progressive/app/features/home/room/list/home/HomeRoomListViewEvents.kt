/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.home.room.list.home

import chat.progressive.app.core.platform.ProgressiveViewEvents
import org.matrix.android.sdk.api.session.room.model.RoomSummary

sealed class HomeRoomListViewEvents : ProgressiveViewEvents {
    data class Loading(val message: CharSequence? = null) : HomeRoomListViewEvents()
    data class Failure(val throwable: Throwable) : HomeRoomListViewEvents()
    object Done : HomeRoomListViewEvents()
    data class SelectRoom(val roomSummary: RoomSummary, val isInviteAlreadyAccepted: Boolean = false) : HomeRoomListViewEvents()
}
