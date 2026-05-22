/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.widgets.permissions

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class RoomWidgetPermissionViewEvents : ProgressiveViewEvents {
    object Close : RoomWidgetPermissionViewEvents()
}
