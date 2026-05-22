/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.navigation

import chat.progressive.app.test.fakes.FakeActiveSessionHolder
import chat.progressive.app.test.fakes.FakeAnalyticsTracker
import chat.progressive.app.test.fakes.FakeContext
import chat.progressive.app.test.fakes.FakeDebugNavigator
import chat.progressive.app.test.fakes.FakeSpaceStateHandler
import chat.progressive.app.test.fakes.FakeSupportedVerificationMethodsProvider
import chat.progressive.app.test.fakes.FakeProgressiveFeatures
import chat.progressive.app.test.fakes.FakeProgressivePreferences
import chat.progressive.app.test.fakes.FakeWidgetArgsBuilder
import chat.progressive.app.test.fixtures.RoomSummaryFixture.aRoomSummary
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.SupervisorJob
import org.junit.Test

internal class DefaultNavigatorTest {

    private val sessionHolder = FakeActiveSessionHolder()
    private val vectorPreferences = FakeProgressivePreferences()
    private val widgetArgsBuilder = FakeWidgetArgsBuilder()
    private val spaceStateHandler = FakeSpaceStateHandler()
    private val supportedVerificationMethodsProvider = FakeSupportedVerificationMethodsProvider()
    private val features = FakeProgressiveFeatures()
    private val analyticsTracker = FakeAnalyticsTracker()
    private val debugNavigator = FakeDebugNavigator()
    private val coroutineScope = CoroutineScope(SupervisorJob())

    private val navigator = DefaultNavigator(
            sessionHolder.instance,
            vectorPreferences.instance,
            widgetArgsBuilder.instance,
            spaceStateHandler,
            supportedVerificationMethodsProvider.instance,
            features,
            coroutineScope,
            analyticsTracker,
            debugNavigator,
    )

    /**
     * The below test is by no means all that we want to test in [DefaultNavigator].
     * Please add relevant tests as you make changes to or related to other functions in the class.
     */

    @Test
    fun `when switchToSpace, then current space set`() {
        val spaceId = "space-id"
        val spaceSummary = aRoomSummary(spaceId)
        sessionHolder.fakeSession.fakeRoomService.getRoomSummaryReturns(spaceSummary)

        navigator.switchToSpace(FakeContext().instance, spaceId, Navigator.PostSwitchSpaceAction.None)

        spaceStateHandler.verifySetCurrentSpace(spaceId)
    }
}
