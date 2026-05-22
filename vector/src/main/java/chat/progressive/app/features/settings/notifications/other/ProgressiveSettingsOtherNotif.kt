/*
 * Copyright 2023, 2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.notifications.other

import chat.progressive.app.R
import chat.progressive.app.core.preference.ProgressivePreferenceCategory
import chat.progressive.app.features.settings.notifications.ProgressiveSettingsPushRule
import chat.progressive.lib.strings.CommonStrings
import org.matrix.android.sdk.api.session.pushrules.RuleIds

class ProgressiveSettingsOtherNotif :
        ProgressiveSettingsPushRule() {

    override var titleRes: Int = CommonStrings.settings_notification_other

    override val preferenceXmlRes = R.xml.progressive_settings_notification_other

    override val prefKeyToPushRuleId = mapOf(
            "SETTINGS_PUSH_RULE_INVITED_TO_ROOM_PREFERENCE_KEY" to RuleIds.RULE_ID_INVITE_ME,
            "SETTINGS_PUSH_RULE_CALL_INVITATIONS_PREFERENCE_KEY" to RuleIds.RULE_ID_CALL,
            "SETTINGS_PUSH_RULE_MESSAGES_SENT_BY_BOT_PREFERENCE_KEY" to RuleIds.RULE_ID_SUPPRESS_BOTS_NOTIFICATIONS,
            "SETTINGS_PUSH_RULE_ROOMS_UPGRADED_KEY" to RuleIds.RULE_ID_TOMBSTONE
    )

    override fun bindPref() {
        super.bindPref()
        val category = findPreference<ProgressivePreferenceCategory>("SETTINGS_OTHER")!!
        category.isIconSpaceReserved = false
    }
}
