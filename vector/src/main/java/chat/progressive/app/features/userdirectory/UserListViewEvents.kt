/*
 * Copyright 2020-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.userdirectory

import chat.progressive.app.core.platform.ProgressiveViewEvents
import chat.progressive.app.features.discovery.ServerAndPolicies

/**
 * Transient events for invite users to room screen.
 */
sealed class UserListViewEvents : ProgressiveViewEvents {
    data class Failure(val throwable: Throwable) : UserListViewEvents()
    data class OnPoliciesRetrieved(val identityServerWithTerms: ServerAndPolicies?) : UserListViewEvents()
    data class OpenShareMatrixToLink(val link: String) : UserListViewEvents()
}
