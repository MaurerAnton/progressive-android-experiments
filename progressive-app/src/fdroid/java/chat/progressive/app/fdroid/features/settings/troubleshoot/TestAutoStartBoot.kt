/*
 * Copyright 2018-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */
package chat.progressive.app.fdroid.features.settings.troubleshoot

import chat.progressive.app.core.resources.StringProvider
import chat.progressive.app.features.settings.ProgressiveBasePreferences
import chat.progressive.app.features.settings.troubleshoot.TroubleshootTest
import chat.progressive.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * Test that the application is started on boot
 */
class TestAutoStartBoot @Inject constructor(
        private val vectorPreferences: ProgressiveBasePreferences,
        private val stringProvider: StringProvider
) :
        TroubleshootTest(CommonStrings.settings_troubleshoot_test_service_boot_title) {

    override fun perform(testParameters: TestParameters) {
        if (vectorPreferences.autoStartOnBoot()) {
            description = stringProvider.getString(CommonStrings.settings_troubleshoot_test_service_boot_success)
            status = TestStatus.SUCCESS
            quickFix = null
        } else {
            description = stringProvider.getString(CommonStrings.settings_troubleshoot_test_service_boot_failed)
            quickFix = object : TroubleshootQuickFix(CommonStrings.settings_troubleshoot_test_service_boot_quickfix) {
                override fun doFix() {
                    vectorPreferences.setAutoStartOnBoot(true)
                    manager?.retry(testParameters)
                }
            }
            status = TestStatus.FAILED
        }
    }
}
