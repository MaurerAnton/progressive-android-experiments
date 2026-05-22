/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.rageshake

import com.airbnb.mvrx.MavericksState

data class BugReportState(
        val serverVersion: String = ""
) : MavericksState
