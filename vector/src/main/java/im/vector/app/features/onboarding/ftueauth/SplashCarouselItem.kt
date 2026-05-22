/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.onboarding.ftueauth

import android.widget.ImageView
import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R
import im.vector.app.core.epoxy.ProgressiveEpoxyHolder
import im.vector.app.core.epoxy.ProgressiveEpoxyModel

@EpoxyModelClass
abstract class SplashCarouselItem : ProgressiveEpoxyModel<SplashCarouselItem.Holder>(R.layout.item_splash_carousel) {

    @EpoxyAttribute
    lateinit var item: SplashCarouselState.Item

    override fun bind(holder: Holder) {
        super.bind(holder)

        holder.view.setBackgroundResource(item.pageBackground)
        holder.image.setImageResource(item.image)
        holder.title.text = item.title.charSequence
        holder.body.setText(item.body)
    }

    class Holder : ProgressiveEpoxyHolder() {
        val image by bind<ImageView>(R.id.carousel_item_image)
        val title by bind<TextView>(R.id.carousel_item_title)
        val body by bind<TextView>(R.id.carousel_item_body)
    }
}
