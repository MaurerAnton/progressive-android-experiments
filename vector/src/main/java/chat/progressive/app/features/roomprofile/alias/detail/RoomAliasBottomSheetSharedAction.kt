/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.roomprofile.alias.detail

import androidx.annotation.DrawableRes
import androidx.annotation.StringRes
import chat.progressive.app.R
import chat.progressive.app.core.platform.ProgressiveSharedAction
import chat.progressive.lib.strings.CommonStrings

sealed class RoomAliasBottomSheetSharedAction(
        @StringRes val titleRes: Int,
        @DrawableRes val iconResId: Int = 0,
        val destructive: Boolean = false
) :
        ProgressiveSharedAction {

    data class ShareAlias(val matrixTo: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.action_share,
            R.drawable.ic_material_share
    )

    data class PublishAlias(val alias: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.room_alias_action_publish
    )

    data class UnPublishAlias(val alias: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.room_alias_action_unpublish
    )

    data class DeleteAlias(val alias: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.action_delete,
            R.drawable.ic_trash_24,
            true
    )

    data class SetMainAlias(val alias: String) : RoomAliasBottomSheetSharedAction(
            CommonStrings.room_settings_set_main_address
    )

    object UnsetMainAlias : RoomAliasBottomSheetSharedAction(
            CommonStrings.room_settings_unset_main_address
    )
}
