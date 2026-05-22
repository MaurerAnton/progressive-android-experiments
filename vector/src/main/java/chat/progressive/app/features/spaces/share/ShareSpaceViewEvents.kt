/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.spaces.share

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class ShareSpaceViewEvents : ProgressiveViewEvents {
    data class NavigateToInviteUser(val spaceId: String) : ShareSpaceViewEvents()
    data class ShowInviteByLink(val permalink: String, val spaceName: String) : ShareSpaceViewEvents()
}
