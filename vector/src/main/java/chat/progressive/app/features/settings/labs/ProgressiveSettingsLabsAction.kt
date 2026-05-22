/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.labs

import chat.progressive.app.core.platform.ProgressiveViewModelAction

sealed class ProgressiveSettingsLabsAction : ProgressiveViewModelAction {
    object UpdateClientInfo : ProgressiveSettingsLabsAction()
    object DeleteRecordedClientInfo : ProgressiveSettingsLabsAction()
}
