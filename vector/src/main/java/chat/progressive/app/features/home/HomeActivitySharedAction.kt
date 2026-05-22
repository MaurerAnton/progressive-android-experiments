/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.home

import chat.progressive.app.core.platform.ProgressiveSharedAction

/**
 * Supported navigation actions for [HomeActivity].
 */
sealed class HomeActivitySharedAction : ProgressiveSharedAction {
    object OpenDrawer : HomeActivitySharedAction()
    object CloseDrawer : HomeActivitySharedAction()
    object OnCloseSpace : HomeActivitySharedAction()
    object AddSpace : HomeActivitySharedAction()
    data class OpenSpacePreview(val spaceId: String) : HomeActivitySharedAction()
    data class OpenSpaceInvite(val spaceId: String) : HomeActivitySharedAction()
    data class ShowSpaceSettings(val spaceId: String) : HomeActivitySharedAction()
    object SendSpaceFeedBack : HomeActivitySharedAction()
}
