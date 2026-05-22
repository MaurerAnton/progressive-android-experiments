/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package chat.progressive.app.features.attachments

import chat.progressive.app.core.platform.ProgressiveSharedAction
import chat.progressive.app.core.platform.ProgressiveSharedAction
import javax.inject.Inject

class AttachmentTypeSelectorSharedActionViewModel @Inject constructor() :
        ProgressiveSharedAction<AttachmentTypeSelectorSharedAction>()

sealed interface AttachmentTypeSelectorSharedAction : ProgressiveSharedAction {
    data class SelectAttachmentTypeAction(
            val attachmentType: AttachmentType
    ) : AttachmentTypeSelectorSharedAction
}
