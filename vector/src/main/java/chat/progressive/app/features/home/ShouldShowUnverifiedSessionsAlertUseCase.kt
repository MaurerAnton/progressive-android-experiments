/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.home

import chat.progressive.app.config.Config
import chat.progressive.app.features.ProgressiveFeatures
import chat.progressive.app.features.settings.ProgressiveBasePreferences
import chat.progressive.lib.core.utils.timer.Clock
import javax.inject.Inject

class ShouldShowUnverifiedSessionsAlertUseCase @Inject constructor(
        private val vectorFeatures: ProgressiveFeatures,
        private val progressivePreferences: ProgressiveBasePreferences,
        private val clock: Clock,
) {

    fun execute(deviceId: String?): Boolean {
        deviceId ?: return false

        val isUnverifiedSessionsAlertEnabled = vectorFeatures.isUnverifiedSessionsAlertEnabled()
        val unverifiedSessionsAlertLastShownMillis = progressivePreferences.getUnverifiedSessionsAlertLastShownMillis(deviceId)
        return isUnverifiedSessionsAlertEnabled &&
                clock.epochMillis() - unverifiedSessionsAlertLastShownMillis >= Config.SHOW_UNVERIFIED_SESSIONS_ALERT_AFTER_MILLIS
    }
}
