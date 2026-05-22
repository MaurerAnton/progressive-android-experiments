/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.home.room.list

import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R
import im.vector.app.core.epoxy.ProgressiveEpoxyHolder
import im.vector.app.core.epoxy.ProgressiveEpoxyModel

@EpoxyModelClass
abstract class SpaceDirectoryFilterNoResultsItem : ProgressiveEpoxyModel<SpaceDirectoryFilterNoResultsItem.Holder>(R.layout.item_space_directory_filter_no_results) {
    class Holder : ProgressiveEpoxyHolder()
}
