/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.analytics

import chat.progressive.app.features.analytics.itf.ProgressiveAnalyticsEvent
import chat.progressive.app.features.analytics.itf.ProgressiveAnalyticsScreen
import chat.progressive.app.features.analytics.plan.SuperProperties
import chat.progressive.app.features.analytics.plan.UserProperties

interface AnalyticsTracker {
    /**
     * Capture an Event.
     */
    fun capture(event: ProgressiveAnalyticsEvent)

    /**
     * Track a displayed screen.
     */
    fun screen(screen: ProgressiveAnalyticsScreen)

    /**
     * Update user specific properties.
     */
    fun updateUserProperties(userProperties: UserProperties)

    /**
     * Update the super properties.
     * Super properties are added to any tracked event automatically.
     */
    fun updateSuperProperties(updatedProperties: SuperProperties)
}
