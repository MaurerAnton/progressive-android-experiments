/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.home.room.list

import chat.progressive.app.features.home.RoomListDisplayMode
import chat.progressive.app.features.settings.FontScalePreferences
import javax.inject.Inject

class RoomSummaryPagedControllerFactory @Inject constructor(
        private val roomSummaryItemFactory: RoomSummaryItemFactory,
        private val fontScalePreferences: FontScalePreferences
) {

    fun createRoomSummaryPagedController(displayMode: RoomListDisplayMode): RoomSummaryPagedController {
        return RoomSummaryPagedController(roomSummaryItemFactory, displayMode, fontScalePreferences)
    }

    fun createRoomSummaryListController(displayMode: RoomListDisplayMode): RoomSummaryListController {
        return RoomSummaryListController(roomSummaryItemFactory, displayMode, fontScalePreferences)
    }

    fun createSuggestedRoomListController() = SuggestedRoomListController(roomSummaryItemFactory)
}
