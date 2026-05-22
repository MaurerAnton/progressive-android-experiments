/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.core.ui.bottomsheet

import chat.progressive.app.core.epoxy.bottomsheet.BottomSheetRadioActionItem_
import chat.progressive.app.core.platform.ProgressiveSharedAction

/**
 * Parent class for a bottom sheet action.
 */
open class BottomSheetGenericRadioAction(
        open val title: String?,
        open val description: String? = null,
        open val isSelected: Boolean
) : ProgressiveSharedAction {

    fun toRadioBottomSheetItem(): BottomSheetRadioActionItem_ {
        return BottomSheetRadioActionItem_().also {
            it.id("action_$title")
            it.title(title)
            it.selected(isSelected)
            it.description(description)
        }
    }
}
