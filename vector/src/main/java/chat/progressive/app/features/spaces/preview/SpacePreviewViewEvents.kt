/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.spaces.preview

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class SpacePreviewViewEvents : ProgressiveViewEvents {
    object Dismiss : SpacePreviewViewEvents()
    object JoinSuccess : SpacePreviewViewEvents()
    data class JoinFailure(val message: String?) : SpacePreviewViewEvents()
}
