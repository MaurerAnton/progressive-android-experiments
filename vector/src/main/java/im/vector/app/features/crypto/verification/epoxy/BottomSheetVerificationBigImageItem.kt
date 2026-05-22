/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package im.vector.app.features.crypto.verification.epoxy

import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R
import im.vector.app.core.epoxy.ProgressiveEpoxyHolder
import im.vector.app.core.epoxy.ProgressiveEpoxyModel
import im.vector.app.core.ui.views.ShieldImageView
import org.matrix.android.sdk.api.session.crypto.model.RoomEncryptionTrustLevel

/**
 * A action for bottom sheet.
 */
@EpoxyModelClass
abstract class BottomSheetVerificationBigImageItem : ProgressiveEpoxyModel<BottomSheetVerificationBigImageItem.Holder>(R.layout.item_verification_big_image) {

    @EpoxyAttribute
    lateinit var roomEncryptionTrustLevel: RoomEncryptionTrustLevel

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.image.render(roomEncryptionTrustLevel, borderLess = true)
    }

    class Holder : ProgressiveEpoxyHolder() {
        val image by bind<ShieldImageView>(R.id.itemVerificationBigImage)
    }
}
