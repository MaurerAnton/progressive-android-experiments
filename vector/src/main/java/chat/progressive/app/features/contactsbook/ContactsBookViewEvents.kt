/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.contactsbook

import chat.progressive.app.core.platform.ProgressiveViewEvents
import chat.progressive.app.features.discovery.ServerAndPolicies

sealed class ContactsBookViewEvents : ProgressiveViewEvents {
    data class Failure(val throwable: Throwable) : ContactsBookViewEvents()
    data class OnPoliciesRetrieved(val identityServerWithTerms: ServerAndPolicies?) : ContactsBookViewEvents()
}
