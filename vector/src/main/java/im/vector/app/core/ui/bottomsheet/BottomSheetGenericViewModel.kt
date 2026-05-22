/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.core.ui.bottomsheet

import com.airbnb.mvrx.MavericksState
import im.vector.app.core.platform.EmptyAction
import im.vector.app.core.platform.EmptyViewEvents
import im.vector.app.core.platform.ProgressiveViewModel

abstract class BottomSheetGenericViewModel<State : MavericksState>(initialState: State) :
        ProgressiveViewModel<State, EmptyAction, EmptyViewEvents>(initialState) {

    override fun handle(action: EmptyAction) {
        // No op
    }
}
