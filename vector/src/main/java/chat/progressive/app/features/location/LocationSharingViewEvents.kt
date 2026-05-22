/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.location

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class LocationSharingViewEvents : ProgressiveViewEvents {
    object Close : LocationSharingViewEvents()
    object LocationNotAvailableError : LocationSharingViewEvents()
    data class ZoomToUserLocation(val userLocation: LocationData) : LocationSharingViewEvents()
    data class StartLiveLocationService(val sessionId: String, val roomId: String, val durationMillis: Long) : LocationSharingViewEvents()
    object ChooseLiveLocationDuration : LocationSharingViewEvents()
    object ShowLabsFlagPromotion : LocationSharingViewEvents()
    object LiveLocationSharingNotEnoughPermission : LocationSharingViewEvents()
}
