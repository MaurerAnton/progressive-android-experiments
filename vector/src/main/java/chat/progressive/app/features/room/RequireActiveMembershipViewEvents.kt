/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.room

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class RequireActiveMembershipViewEvents : ProgressiveViewEvents {
    data class RoomLeft(val leftMessage: String?) : RequireActiveMembershipViewEvents()
}
