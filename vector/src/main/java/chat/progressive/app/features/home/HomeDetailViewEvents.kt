/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.home

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class HomeDetailViewEvents : ProgressiveViewEvents {
    object Loading : HomeDetailViewEvents()
    object CallStarted : HomeDetailViewEvents()
    data class FailToCall(val failure: Throwable) : HomeDetailViewEvents()
}
