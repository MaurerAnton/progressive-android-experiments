/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.roomprofile

import im.vector.app.core.platform.VectorSharedAction

/**
 * Supported navigation actions for [RoomProfileActivity].
 */
sealed class RoomProfileSharedAction : VectorSharedAction {
    object OpenRoomSettings : RoomProfileSharedAction()
    object OpenRoomAliasesSettings : RoomProfileSharedAction()
    object OpenRoomPermissionsSettings : RoomProfileSharedAction()
    object OpenRoomPolls : RoomProfileSharedAction()
    object OpenRoomUploads : RoomProfileSharedAction()
    object OpenRoomMembers : RoomProfileSharedAction()
    object OpenBannedRoomMembers : RoomProfileSharedAction()
    object OpenRoomNotificationSettings : RoomProfileSharedAction()
}
