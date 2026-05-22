/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.settings.devices.v2

import im.vector.app.features.settings.VectorPreferences
import javax.inject.Inject

class ToggleIpAddressVisibilityUseCase @Inject constructor(
        private val vectorPreferences: VectorPreferences,
) {

    fun execute() {
        val currentVisibility = vectorPreferences.showIpAddressInSessionManagerScreens()
        vectorPreferences.setIpAddressVisibilityInDeviceManagerScreens(!currentVisibility)
    }
}
