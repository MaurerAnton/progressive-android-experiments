/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.devtools

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class DevToolsViewEvents : ProgressiveViewEvents {
    object Dismiss : DevToolsViewEvents()

    //    object ShowStateList : DevToolsViewEvents()
    data class ShowAlertMessage(val message: String) : DevToolsViewEvents()
    data class ShowSnackMessage(val message: String) : DevToolsViewEvents()
}
