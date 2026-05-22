/*
 * Copyright 2018-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.consent

import android.app.Activity
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import chat.progressive.app.core.dialogs.DialogLocker
import chat.progressive.app.core.platform.Restorable
import chat.progressive.app.features.webview.ProgressiveWebViewActivity
import chat.progressive.app.features.webview.WebViewMode
import chat.progressive.lib.strings.CommonStrings

class ConsentNotGivenHelper(
        private val activity: Activity,
        private val dialogLocker: DialogLocker
) :
        Restorable by dialogLocker {

    /* ==========================================================================================
     * Public methods
     * ========================================================================================== */

    /**
     * Display the consent dialog, if not already displayed.
     */
    fun displayDialog(consentUri: String, homeServerHost: String) {
        dialogLocker.displayDialog {
            MaterialAlertDialogBuilder(activity)
                    .setTitle(CommonStrings.settings_app_term_conditions)
                    .setMessage(activity.getString(CommonStrings.dialog_user_consent_content, homeServerHost))
                    .setPositiveButton(CommonStrings.dialog_user_consent_submit) { _, _ ->
                        openWebViewActivity(consentUri)
                    }
        }
    }

    /* ==========================================================================================
     * Private
     * ========================================================================================== */

    private fun openWebViewActivity(consentUri: String) {
        val intent = ProgressiveWebViewActivity.getIntent(activity, consentUri, activity.getString(CommonStrings.settings_app_term_conditions), WebViewMode.CONSENT)
        activity.startActivity(intent)
    }
}
