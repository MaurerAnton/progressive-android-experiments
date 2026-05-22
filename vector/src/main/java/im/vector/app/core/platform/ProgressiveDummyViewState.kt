/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.core.platform

import com.airbnb.mvrx.MavericksState

data class ProgressiveDummyViewState(
        val isDummy: Unit = Unit
) : MavericksState
