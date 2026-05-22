/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package im.vector.app.features.crypto.verification.epoxy

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R
import im.vector.app.core.epoxy.ProgressiveEpoxyHolder
import im.vector.app.core.epoxy.ProgressiveEpoxyModel

/**
 * A action for bottom sheet.
 */
@EpoxyModelClass
abstract class BottomSheetVerificationWaitingItem : ProgressiveEpoxyModel<BottomSheetVerificationWaitingItem.Holder>(R.layout.item_verification_waiting) {

    @EpoxyAttribute
    var title: String = ""

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.title.text = title
    }

    class Holder : ProgressiveEpoxyHolder() {
        val title by bind<TextView>(R.id.itemVerificationWaitingTitle)
    }
}
