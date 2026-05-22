/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.home.room.list.home.invites

import chat.progressive.app.core.platform.ProgressiveViewEvents
import org.matrix.android.sdk.api.session.room.model.RoomSummary

sealed class InvitesViewEvents : ProgressiveViewEvents {
    data class Failure(val throwable: Throwable) : InvitesViewEvents()
    data class OpenRoom(
            val roomSummary: RoomSummary,
            val shouldCloseInviteView: Boolean,
            val isInviteAlreadySelected: Boolean,
    ) : InvitesViewEvents()
}
