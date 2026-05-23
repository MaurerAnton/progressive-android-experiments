/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package chat.progressive.app.features.discovery

import android.widget.TextView
import androidx.annotation.DrawableRes
import androidx.annotation.StringRes
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import chat.progressive.app.R
import chat.progressive.app.core.epoxy.ClickListener
import chat.progressive.app.core.epoxy.ProgressiveEpoxyHolder
import chat.progressive.app.core.epoxy.ProgressiveEpoxyModel
import chat.progressive.app.core.epoxy.onClick
import chat.progressive.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class SettingsInfoItem : ProgressiveEpoxyModel<SettingsInfoItem.Holder>(R.layout.item_settings_helper_info) {

    @EpoxyAttribute
    var helperText: String? = null

    @EpoxyAttribute
    @StringRes
    var helperTextResId: Int? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var itemClickListener: ClickListener? = null

    @EpoxyAttribute
    @DrawableRes
    var compoundDrawable: Int = R.drawable.progressive_warning_red

    @EpoxyAttribute
    var showCompoundDrawable: Boolean = false

    override fun bind(holder: Holder) {
        super.bind(holder)

        if (helperTextResId != null) {
            holder.text.setText(helperTextResId!!)
        } else {
            holder.text.setTextOrHide(helperText)
        }

        holder.view.onClick(itemClickListener)

        if (showCompoundDrawable) {
            holder.text.setCompoundDrawablesWithIntrinsicBounds(compoundDrawable, 0, 0, 0)
        } else {
            holder.text.setCompoundDrawablesWithIntrinsicBounds(0, 0, 0, 0)
        }
    }

    class Holder : ProgressiveEpoxyHolder() {
        val text by bind<TextView>(R.id.settings_helper_text)
    }
}
