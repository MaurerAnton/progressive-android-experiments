/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.core.resources

import chat.progressive.app.features.settings.ProgressiveBasePreferences
import javax.inject.Inject

class UserPreferencesProvider @Inject constructor(private val progressivePreferences: ProgressiveBasePreferences) {

    fun shouldShowHiddenEvents() = progressivePreferences.shouldShowHiddenEvents()

    fun shouldShowReadReceipts() = progressivePreferences.showReadReceipts()

    fun shouldShowRedactedMessages() = progressivePreferences.showRedactedMessages()

    fun shouldShowLongClickOnRoomHelp() = progressivePreferences.shouldShowLongClickOnRoomHelp()

    fun neverShowLongClickOnRoomHelpAgain() {
        progressivePreferences.neverShowLongClickOnRoomHelpAgain()
    }

    fun shouldShowJoinLeaves() = progressivePreferences.showJoinLeaveMessages()

    fun shouldShowAvatarDisplayNameChanges() = progressivePreferences.showAvatarDisplayNameChangeMessages()

    fun areThreadMessagesEnabled() = progressivePreferences.areThreadMessagesEnabled()

    fun showLiveSenderInfo() = progressivePreferences.showLiveSenderInfo()

    fun autoplayAnimatedImages() = progressivePreferences.autoplayAnimatedImages()
}
