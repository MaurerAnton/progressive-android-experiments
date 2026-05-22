/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.home.room.list.actions

import chat.progressive.app.core.platform.ProgressiveSharedAction

sealed class RoomListSharedAction : ProgressiveSharedAction {

    object CloseBottomSheet : RoomListSharedAction()
}
