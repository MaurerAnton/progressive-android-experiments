/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package im.vector.app.features.crypto.verification.epoxy

import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R
import im.vector.app.core.epoxy.ProgressiveEpoxyHolder
import im.vector.app.core.epoxy.ProgressiveEpoxyModel

/**
 * A action for bottom sheet.
 */
@EpoxyModelClass
abstract class BottomSheetSelfWaitItem : ProgressiveEpoxyModel<BottomSheetSelfWaitItem.Holder>(R.layout.item_verification_wait) {
    class Holder : ProgressiveEpoxyHolder()
}
