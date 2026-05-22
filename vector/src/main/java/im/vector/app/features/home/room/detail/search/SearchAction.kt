/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.home.room.detail.search

import im.vector.app.core.platform.ProgressiveViewModelAction

sealed class SearchAction : ProgressiveViewModelAction {
    data class SearchWith(val searchTerm: String) : SearchAction()
    object LoadMore : SearchAction()
    object Retry : SearchAction()
}
