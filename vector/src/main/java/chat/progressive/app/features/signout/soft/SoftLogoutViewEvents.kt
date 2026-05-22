/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.signout.soft

import chat.progressive.app.core.platform.ProgressiveViewEvents

/**
 * Transient events for SoftLogout.
 */
sealed class SoftLogoutViewEvents : ProgressiveViewEvents {
    data class Failure(val throwable: Throwable) : SoftLogoutViewEvents()

    data class ErrorNotSameUser(val currentUserId: String, val newUserId: String) : SoftLogoutViewEvents()
    object ClearData : SoftLogoutViewEvents()
}
