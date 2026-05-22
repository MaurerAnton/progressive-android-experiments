/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.core.epoxy.profiles

import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R
import im.vector.app.core.epoxy.ProgressiveEpoxyHolder
import im.vector.app.core.epoxy.ProgressiveEpoxyModel

@EpoxyModelClass
abstract class ProfileSectionItem : ProgressiveEpoxyModel<ProfileSectionItem.Holder>(R.layout.item_profile_section) {

    @EpoxyAttribute
    lateinit var title: String

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.sectionView.text = title
    }

    class Holder : ProgressiveEpoxyHolder() {
        val sectionView by bind<TextView>(R.id.itemProfileSectionView)
    }
}
