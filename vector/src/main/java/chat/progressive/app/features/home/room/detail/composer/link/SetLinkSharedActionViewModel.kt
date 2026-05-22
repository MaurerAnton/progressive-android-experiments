/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package chat.progressive.app.features.home.room.detail.composer.link

import chat.progressive.app.core.platform.ProgressiveSharedAction
import chat.progressive.app.core.platform.ProgressiveSharedAction
import javax.inject.Inject

class SetLinkSharedActionViewModel @Inject constructor() :
        ProgressiveSharedAction<SetLinkSharedAction>()

sealed interface SetLinkSharedAction : ProgressiveSharedAction {
    data class Set(
            val link: String,
    ) : SetLinkSharedAction

    data class Insert(
            val text: String,
            val link: String,
    ) : SetLinkSharedAction

    object Remove : SetLinkSharedAction
}
