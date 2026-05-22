/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.home.room.detail.timeline.item

import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R
import im.vector.app.core.epoxy.ProgressiveEpoxyHolder
import im.vector.app.core.epoxy.ProgressiveEpoxyModel

@EpoxyModelClass
abstract class TimelineReadMarkerItem : ProgressiveEpoxyModel<TimelineReadMarkerItem.Holder>(R.layout.item_timeline_read_marker) {

    class Holder : ProgressiveEpoxyHolder()
}
