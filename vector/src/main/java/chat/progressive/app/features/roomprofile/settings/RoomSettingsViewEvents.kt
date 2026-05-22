/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.roomprofile.settings

import chat.progressive.app.core.platform.ProgressiveViewEvents

/**
 * Transient events for room settings screen.
 */
sealed class RoomSettingsViewEvents : ProgressiveViewEvents {
    data class Failure(val throwable: Throwable) : RoomSettingsViewEvents()
    object Success : RoomSettingsViewEvents()
    object GoBack : RoomSettingsViewEvents()
}
