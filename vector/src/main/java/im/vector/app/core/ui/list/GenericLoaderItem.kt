/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.core.ui.list

import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R
import im.vector.app.core.epoxy.ProgressiveEpoxyHolder
import im.vector.app.core.epoxy.ProgressiveEpoxyModel

/**
 * A generic list item header left aligned with notice color.
 */
@EpoxyModelClass
abstract class GenericLoaderItem : ProgressiveEpoxyModel<GenericLoaderItem.Holder>(R.layout.item_generic_loader) {

    // Maybe/Later add some style configuration, SMALL/BIG ?

    class Holder : ProgressiveEpoxyHolder()
}
