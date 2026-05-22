/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.home.room.detail.timeline.helper

import com.airbnb.epoxy.VisibilityState
import im.vector.app.core.epoxy.ProgressiveEpoxyModel
import im.vector.app.features.home.room.detail.timeline.TimelineEventController
import org.matrix.android.sdk.api.session.room.timeline.TimelineEvent

class ReadMarkerVisibilityStateChangedListener(private val callback: TimelineEventController.Callback?) :
        ProgressiveEpoxyModel.OnVisibilityStateChangedListener {

    override fun onVisibilityStateChanged(visibilityState: Int) {
        if (visibilityState == VisibilityState.VISIBLE) {
            callback?.onReadMarkerVisible()
        }
    }
}

class TimelineEventVisibilityStateChangedListener(
        private val callback: TimelineEventController.Callback?,
        private val event: TimelineEvent
) :
        ProgressiveEpoxyModel.OnVisibilityStateChangedListener {

    override fun onVisibilityStateChanged(visibilityState: Int) {
        if (visibilityState == VisibilityState.VISIBLE) {
            callback?.onEventVisible(event)
        } else if (visibilityState == VisibilityState.INVISIBLE) {
            callback?.onEventInvisible(event)
        }
    }
}

class MergedTimelineEventVisibilityStateChangedListener(
        private val callback: TimelineEventController.Callback?,
        private val events: List<TimelineEvent>
) :
        ProgressiveEpoxyModel.OnVisibilityStateChangedListener {

    override fun onVisibilityStateChanged(visibilityState: Int) {
        if (visibilityState == VisibilityState.VISIBLE) {
            events.forEach { callback?.onEventVisible(it) }
        } else if (visibilityState == VisibilityState.INVISIBLE) {
            events.forEach { callback?.onEventInvisible(it) }
        }
    }
}
