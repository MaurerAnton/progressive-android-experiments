/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.discovery.change

import androidx.annotation.StringRes
import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class SetIdentityServerViewEvents : ProgressiveViewEvents {
    data class Loading(val message: CharSequence? = null) : SetIdentityServerViewEvents()
    data class Failure(@StringRes val errorMessageId: Int, val forDefault: Boolean) : SetIdentityServerViewEvents()
    data class OtherFailure(val failure: Throwable) : SetIdentityServerViewEvents()

    data class ShowTerms(val identityServerUrl: String) : SetIdentityServerViewEvents()

    object NoTerms : SetIdentityServerViewEvents()
    object TermsAccepted : SetIdentityServerViewEvents()
}
