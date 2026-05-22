/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package im.vector.app.features.attachments

import im.vector.app.core.platform.VectorSharedAction
import im.vector.app.core.platform.ProgressiveSharedAction
import javax.inject.Inject

class AttachmentTypeSelectorSharedActionViewModel @Inject constructor() :
        ProgressiveSharedAction<AttachmentTypeSelectorSharedAction>()

sealed interface AttachmentTypeSelectorSharedAction : VectorSharedAction {
    data class SelectAttachmentTypeAction(
            val attachmentType: AttachmentType
    ) : AttachmentTypeSelectorSharedAction
}
