/*
 * Copyright 2018-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */
package chat.progressive.app.push.fcm

import androidx.fragment.app.Fragment
import chat.progressive.app.core.pushers.UnifiedPushHelper
import chat.progressive.app.fdroid.features.settings.troubleshoot.TestAutoStartBoot
import chat.progressive.app.fdroid.features.settings.troubleshoot.TestBackgroundRestrictions
import chat.progressive.app.fdroid.features.settings.troubleshoot.TestBatteryOptimization
import chat.progressive.app.features.VectorFeatures
import chat.progressive.app.features.push.NotificationTroubleshootTestManagerFactory
import chat.progressive.app.features.settings.troubleshoot.NotificationTroubleshootTestManager
import chat.progressive.app.features.settings.troubleshoot.TestAccountSettings
import chat.progressive.app.features.settings.troubleshoot.TestAvailableUnifiedPushDistributors
import chat.progressive.app.features.settings.troubleshoot.TestCurrentUnifiedPushDistributor
import chat.progressive.app.features.settings.troubleshoot.TestDeviceSettings
import chat.progressive.app.features.settings.troubleshoot.TestEndpointAsTokenRegistration
import chat.progressive.app.features.settings.troubleshoot.TestNotification
import chat.progressive.app.features.settings.troubleshoot.TestPushFromPushGateway
import chat.progressive.app.features.settings.troubleshoot.TestPushRulesSettings
import chat.progressive.app.features.settings.troubleshoot.TestSystemSettings
import chat.progressive.app.features.settings.troubleshoot.TestUnifiedPushEndpoint
import chat.progressive.app.features.settings.troubleshoot.TestUnifiedPushGateway
import javax.inject.Inject

class FdroidNotificationTroubleshootTestManagerFactory @Inject constructor(
        private val unifiedPushHelper: UnifiedPushHelper,
        private val testSystemSettings: TestSystemSettings,
        private val testAccountSettings: TestAccountSettings,
        private val testDeviceSettings: TestDeviceSettings,
        private val testPushRulesSettings: TestPushRulesSettings,
        private val testCurrentUnifiedPushDistributor: TestCurrentUnifiedPushDistributor,
        private val testUnifiedPushGateway: TestUnifiedPushGateway,
        private val testUnifiedPushEndpoint: TestUnifiedPushEndpoint,
        private val testAvailableUnifiedPushDistributors: TestAvailableUnifiedPushDistributors,
        private val testEndpointAsTokenRegistration: TestEndpointAsTokenRegistration,
        private val testPushFromPushGateway: TestPushFromPushGateway,
        private val testAutoStartBoot: TestAutoStartBoot,
        private val testBackgroundRestrictions: TestBackgroundRestrictions,
        private val testBatteryOptimization: TestBatteryOptimization,
        private val testNotification: TestNotification,
        private val vectorFeatures: VectorFeatures,
) : NotificationTroubleshootTestManagerFactory {

    override fun create(fragment: Fragment): NotificationTroubleshootTestManager {
        val mgr = NotificationTroubleshootTestManager(fragment)
        mgr.addTest(testSystemSettings)
        mgr.addTest(testAccountSettings)
        mgr.addTest(testDeviceSettings)
        mgr.addTest(testPushRulesSettings)
        if (vectorFeatures.allowExternalUnifiedPushDistributors()) {
            mgr.addTest(testAvailableUnifiedPushDistributors)
            mgr.addTest(testCurrentUnifiedPushDistributor)
        }
        if (unifiedPushHelper.isBackgroundSync()) {
            mgr.addTest(testAutoStartBoot)
            mgr.addTest(testBackgroundRestrictions)
            mgr.addTest(testBatteryOptimization)
        } else {
            mgr.addTest(testUnifiedPushGateway)
            mgr.addTest(testUnifiedPushEndpoint)
            mgr.addTest(testEndpointAsTokenRegistration)
            mgr.addTest(testPushFromPushGateway)
        }
        mgr.addTest(testNotification)
        return mgr
    }
}
