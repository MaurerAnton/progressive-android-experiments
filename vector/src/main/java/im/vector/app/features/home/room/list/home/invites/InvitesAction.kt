/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.home.room.list.home.invites

import im.vector.app.core.platform.ProgressiveViewModelAction
import org.matrix.android.sdk.api.session.room.model.RoomSummary

sealed class InvitesAction : ProgressiveViewModelAction {
    data class SelectRoom(val roomSummary: RoomSummary) : InvitesAction()
    data class AcceptInvitation(val roomSummary: RoomSummary) : InvitesAction()
    data class RejectInvitation(val roomSummary: RoomSummary) : InvitesAction()
}
