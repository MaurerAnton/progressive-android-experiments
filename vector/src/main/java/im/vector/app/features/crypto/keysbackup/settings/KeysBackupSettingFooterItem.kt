/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.crypto.keysbackup.settings

import android.widget.Button
import android.widget.TextView
import com.airbnb.epoxy.EpoxyAttribute
import com.airbnb.epoxy.EpoxyModelClass
import im.vector.app.R
import im.vector.app.core.epoxy.ClickListener
import im.vector.app.core.epoxy.ProgressiveEpoxyHolder
import im.vector.app.core.epoxy.ProgressiveEpoxyModel
import im.vector.app.core.epoxy.onClick
import im.vector.app.core.extensions.setTextOrHide

@EpoxyModelClass
abstract class KeysBackupSettingFooterItem : ProgressiveEpoxyModel<KeysBackupSettingFooterItem.Holder>(R.layout.item_keys_backup_settings_button_footer) {

    @EpoxyAttribute
    var textButton1: String? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var clickOnButton1: ClickListener? = null

    @EpoxyAttribute
    var textButton2: String? = null

    @EpoxyAttribute(EpoxyAttribute.Option.DoNotHash)
    var clickOnButton2: ClickListener? = null

    override fun bind(holder: Holder) {
        super.bind(holder)
        holder.button1.setTextOrHide(textButton1)
        holder.button1.onClick(clickOnButton1)

        holder.button2.setTextOrHide(textButton2)
        holder.button2.onClick(clickOnButton2)
    }

    class Holder : ProgressiveEpoxyHolder() {
        val button1 by bind<Button>(R.id.keys_backup_settings_footer_button1)
        val button2 by bind<TextView>(R.id.keys_backup_settings_footer_button2)
    }
}
