/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.call.conference

import im.vector.app.core.platform.ProgressiveViewModelAction

sealed class JitsiCallViewActions : ProgressiveViewModelAction {
    data class SwitchTo(
            val args: ProgressiveJitsiActivity.Args,
            val withConfirmation: Boolean
    ) : JitsiCallViewActions()

    /**
     * The ViewModel will either ask the View to finish, or to join another conf.
     */
    object OnConferenceLeft : JitsiCallViewActions()
}
