/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.analytics.ui.consent

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed interface AnalyticsOptInViewEvents : ProgressiveViewEvents {
    object OnDataSaved : AnalyticsOptInViewEvents
}
