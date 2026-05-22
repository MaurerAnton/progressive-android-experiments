/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.core.epoxy

import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R

@EpoxyModelClass
abstract class SquareLoadingItem : ProgressiveEpoxyModel<SquareLoadingItem.Holder>(R.layout.item_loading_square) {

    class Holder : ProgressiveEpoxyHolder()
}
