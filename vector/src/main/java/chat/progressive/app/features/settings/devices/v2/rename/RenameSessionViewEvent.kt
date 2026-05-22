/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.devices.v2.rename

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class RenameSessionViewEvent : ProgressiveViewEvents {
    data class Initialized(val deviceName: String) : RenameSessionViewEvent()
    object SessionRenamed : RenameSessionViewEvent()
    data class Failure(val throwable: Throwable) : RenameSessionViewEvent()
}
