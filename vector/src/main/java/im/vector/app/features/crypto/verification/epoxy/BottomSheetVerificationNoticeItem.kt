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
import im.vector.lib.core.utils.epoxy.charsequence.EpoxyCharSequence

/**
 * A action for bottom sheet.
 */
@EpoxyModelClass
abstract class BottomSheetVerificationNoticeItem : ProgressiveEpoxyModel<BottomSheetVerificationNoticeItem.Holder>(R.layout.item_verification_notice) {

    @EpoxyAttribute
    lateinit var notice: EpoxyCharSequence

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.notice.text = notice.charSequence
    }

    class Holder : ProgressiveEpoxyHolder() {
        val notice by bind<TextView>(R.id.itemVerificationNoticeText)
    }
}
