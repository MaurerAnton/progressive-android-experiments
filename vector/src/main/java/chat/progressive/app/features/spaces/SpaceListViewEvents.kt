/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.spaces

import chat.progressive.app.core.platform.ProgressiveViewEvents

/**
 * Transient events for group list screen.
 */
sealed class SpaceListViewEvents : ProgressiveViewEvents {
    data class OpenSpaceSummary(val id: String) : SpaceListViewEvents()
    data class OpenSpaceInvite(val id: String) : SpaceListViewEvents()
    object AddSpace : SpaceListViewEvents()
    object CloseDrawer : SpaceListViewEvents()
}
