/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.call

import chat.progressive.app.features.call.lookup.CallProtocolsChecker
import chat.progressive.app.features.call.lookup.CallUserMapper
import chat.progressive.app.features.session.SessionScopedProperty
import org.matrix.android.sdk.api.session.Session

interface ProgressiveCallService {
    val protocolChecker: CallProtocolsChecker
    val userMapper: CallUserMapper
}

val Session.vectorCallService: ProgressiveCallService by SessionScopedProperty {
    object : ProgressiveCallService {
        override val protocolChecker = CallProtocolsChecker(it)
        override val userMapper = CallUserMapper(it, protocolChecker)
    }
}
