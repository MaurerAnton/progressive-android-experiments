/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.invite

import im.vector.app.core.platform.ProgressiveViewModelAction
import im.vector.app.features.userdirectory.PendingSelection

sealed class InviteUsersToRoomAction : ProgressiveViewModelAction {
    data class InviteSelectedUsers(val selections: Set<PendingSelection>) : InviteUsersToRoomAction()
}
