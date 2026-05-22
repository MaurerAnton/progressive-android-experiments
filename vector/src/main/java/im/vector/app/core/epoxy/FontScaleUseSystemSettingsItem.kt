/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.core.epoxy

import android.widget.CheckBox
import android.widget.CompoundButton
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R

@EpoxyModelClass
abstract class FontScaleUseSystemSettingsItem : ProgressiveEpoxyModel<FontScaleUseSystemSettingsItem.Holder>(R.layout.item_font_scale_system) {

    @EpoxyAttribute var useSystemSettings: Boolean = true

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var checkChangeListener: CompoundButton.OnCheckedChangeListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.checkBox.isChecked = useSystemSettings
        holder.checkBox.setOnCheckedChangeListener(checkChangeListener)
        holder.view.onClick {
            holder.checkBox.performClick()
        }
    }

    class Holder : ProgressiveEpoxyHolder() {
        val checkBox by bind<CheckBox>(R.id.font_scale_use_system_checkbox)
    }
}
