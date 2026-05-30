/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.roomprofile.polls.active

import dagger.hilt.android.AndroidEntryPoint
import chat.progressive.app.features.roomprofile.polls.RoomPollsType
import chat.progressive.app.features.roomprofile.polls.list.ui.RoomPollsListFragment
import chat.progressive.lib.strings.CommonPlurals
import chat.progressive.lib.strings.CommonStrings

@AndroidEntryPoint
class RoomActivePollsFragment : RoomPollsListFragment() {

    override fun getEmptyListTitle(canLoadMore: Boolean, nbLoadedDays: Int): String {
        return if (canLoadMore) {
            stringProvider.getQuantityString(CommonPlurals.room_polls_active_no_item_for_loaded_period, nbLoadedDays, nbLoadedDays)
        } else {
            getString(CommonStrings.room_polls_active_no_item)
        }
    }

    fun getRoomPollsType() = RoomPollsType.ACTIVE
}
