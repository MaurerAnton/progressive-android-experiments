/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.location.preview

import chat.progressive.app.core.platform.ProgressiveViewEvents
import chat.progressive.app.features.location.LocationData

sealed class LocationPreviewViewEvents : ProgressiveViewEvents {
    data class ZoomToUserLocation(val userLocation: LocationData) : LocationPreviewViewEvents()
    object UserLocationNotAvailableError : LocationPreviewViewEvents()
}
