/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.home.room.detail.composer.link

import im.vector.app.core.platform.ProgressiveViewModelAction

sealed class SetLinkAction : ProgressiveViewModelAction {
    data class LinkChanged(
            val newLink: String
    ) : SetLinkAction()

    data class Save(
            val link: String,
            val text: String,
    ) : SetLinkAction()
}
