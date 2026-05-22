/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.auth

import im.vector.app.core.platform.ProgressiveViewModelAction

sealed class ReAuthActions : ProgressiveViewModelAction {
    object StartSSOFallback : ReAuthActions()
    object FallBackPageLoaded : ReAuthActions()
    object FallBackPageClosed : ReAuthActions()
    data class ReAuthWithPass(val password: String) : ReAuthActions()
}
