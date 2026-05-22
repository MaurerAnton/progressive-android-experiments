/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.home.room.detail.timeline.action

import im.vector.app.core.platform.ProgressiveViewModelAction

sealed class MessageActionsAction : ProgressiveViewModelAction {
    object ToggleReportMenu : MessageActionsAction()
}
