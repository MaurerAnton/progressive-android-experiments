/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.media

import im.vector.app.core.platform.ProgressiveViewModelAction
import java.io.File

sealed class VectorAttachmentViewerAction : ProgressiveViewModelAction {
    data class DownloadMedia(val file: File) : VectorAttachmentViewerAction()
}
