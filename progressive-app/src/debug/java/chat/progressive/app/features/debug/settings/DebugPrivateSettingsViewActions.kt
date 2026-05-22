/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.debug.settings

import chat.progressive.app.core.platform.VectorViewModelAction

sealed interface DebugPrivateSettingsViewActions : VectorViewModelAction {
    data class SetDialPadVisibility(val force: Boolean) : DebugPrivateSettingsViewActions
    data class SetForceLoginFallbackEnabled(val force: Boolean) : DebugPrivateSettingsViewActions
    data class SetDisplayNameCapabilityOverride(val option: BooleanHomeserverCapabilitiesOverride?) : DebugPrivateSettingsViewActions
    data class SetAvatarCapabilityOverride(val option: BooleanHomeserverCapabilitiesOverride?) : DebugPrivateSettingsViewActions
    object ResetReleaseNotesActivityHasBeenDisplayed : DebugPrivateSettingsViewActions
}
