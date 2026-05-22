/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.roomprofile.uploads

import chat.progressive.app.core.platform.ProgressiveViewEvents
import java.io.File

sealed class RoomUploadsViewEvents : ProgressiveViewEvents {
    data class Failure(val throwable: Throwable) : RoomUploadsViewEvents()

    data class FileReadyForSharing(val file: File) : RoomUploadsViewEvents()
    data class FileReadyForSaving(val file: File, val title: String) : RoomUploadsViewEvents()
}
