/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.roomprofile.permissions

import chat.progressive.app.core.platform.ProgressiveViewEvents

/**
 * Transient events for room settings screen.
 */
sealed class RoomPermissionsViewEvents : ProgressiveViewEvents {
    data class Failure(val throwable: Throwable) : RoomPermissionsViewEvents()
    object Success : RoomPermissionsViewEvents()
}
