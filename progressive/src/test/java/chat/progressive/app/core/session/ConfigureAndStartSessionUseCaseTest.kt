/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.core.session

import chat.progressive.app.core.extensions.startSyncing
import chat.progressive.app.core.session.clientinfo.UpdateMatrixClientInfoUseCase
import chat.progressive.app.features.session.coroutineScope
import chat.progressive.app.features.settings.devices.v2.notification.UpdateNotificationSettingsAccountDataUseCase
import chat.progressive.app.test.fakes.FakeContext
import chat.progressive.app.test.fakes.FakeNotificationsSettingUpdater
import chat.progressive.app.test.fakes.FakePushRulesUpdater
import chat.progressive.app.test.fakes.FakeSession
import chat.progressive.app.test.fakes.FakeProgressivePreferences
import chat.progressive.app.test.fakes.FakeWebRtcCallManager
import io.mockk.coJustRun
import io.mockk.coVerify
import io.mockk.every
import io.mockk.just
import io.mockk.mockk
import io.mockk.mockkStatic
import io.mockk.runs
import io.mockk.unmockkAll
import io.mockk.verify
import kotlinx.coroutines.test.advanceUntilIdle
import kotlinx.coroutines.test.runTest
import org.junit.After
import org.junit.Before
import org.junit.Test

class ConfigureAndStartSessionUseCaseTest {

    private val fakeContext = FakeContext()
    private val fakeWebRtcCallManager = FakeWebRtcCallManager()
    private val fakeUpdateMatrixClientInfoUseCase = mockk<UpdateMatrixClientInfoUseCase>()
    private val fakeProgressivePreferences = FakeProgressivePreferences()
    private val fakeNotificationsSettingUpdater = FakeNotificationsSettingUpdater()
    private val fakePushRulesUpdater = FakePushRulesUpdater()
    private val fakeUpdateNotificationSettingsAccountDataUseCase = mockk<UpdateNotificationSettingsAccountDataUseCase>()

    private val configureAndStartSessionUseCase = ConfigureAndStartSessionUseCase(
            context = fakeContext.instance,
            webRtcCallManager = fakeWebRtcCallManager.instance,
            updateMatrixClientInfoUseCase = fakeUpdateMatrixClientInfoUseCase,
            vectorPreferences = fakeProgressivePreferences.instance,
            notificationsSettingUpdater = fakeNotificationsSettingUpdater.instance,
            updateNotificationSettingsAccountDataUseCase = fakeUpdateNotificationSettingsAccountDataUseCase,
            pushRulesUpdater = fakePushRulesUpdater.instance,
    )

    @Before
    fun setup() {
        mockkStatic("chat.progressive.app.core.extensions.SessionKt")
        mockkStatic("chat.progressive.app.features.session.SessionCoroutineScopesKt")
    }

    @After
    fun tearDown() {
        unmockkAll()
    }

    @Test
    fun `given start sync needed and client info recording enabled when execute then it should be configured properly`() = runTest {
        // Given
        val aSession = givenASession()
        every { aSession.coroutineScope } returns this
        fakeWebRtcCallManager.givenCheckForProtocolsSupportIfNeededSucceeds()
        coJustRun { fakeUpdateMatrixClientInfoUseCase.execute(any()) }
        coJustRun { fakeUpdateNotificationSettingsAccountDataUseCase.execute(any()) }
        fakeProgressivePreferences.givenIsClientInfoRecordingEnabled(isEnabled = true)
        fakeNotificationsSettingUpdater.givenOnSessionStarted(aSession)
        fakePushRulesUpdater.givenOnSessionStarted(aSession)

        // When
        configureAndStartSessionUseCase.execute(aSession, startSyncing = true)
        advanceUntilIdle()

        // Then
        verify { aSession.startSyncing(fakeContext.instance) }
        aSession.fakePushersService.verifyRefreshPushers()
        fakeWebRtcCallManager.verifyCheckForProtocolsSupportIfNeeded()
        coVerify {
            fakeUpdateMatrixClientInfoUseCase.execute(aSession)
            fakeUpdateNotificationSettingsAccountDataUseCase.execute(aSession)
        }
    }

    @Test
    fun `given start sync needed and client info recording disabled when execute then it should be configured properly`() = runTest {
        // Given
        val aSession = givenASession()
        every { aSession.coroutineScope } returns this
        fakeWebRtcCallManager.givenCheckForProtocolsSupportIfNeededSucceeds()
        coJustRun { fakeUpdateNotificationSettingsAccountDataUseCase.execute(any()) }
        fakeProgressivePreferences.givenIsClientInfoRecordingEnabled(isEnabled = false)
        fakeNotificationsSettingUpdater.givenOnSessionStarted(aSession)
        fakePushRulesUpdater.givenOnSessionStarted(aSession)

        // When
        configureAndStartSessionUseCase.execute(aSession, startSyncing = true)
        advanceUntilIdle()

        // Then
        verify { aSession.startSyncing(fakeContext.instance) }
        aSession.fakePushersService.verifyRefreshPushers()
        fakeWebRtcCallManager.verifyCheckForProtocolsSupportIfNeeded()
        coVerify(inverse = true) {
            fakeUpdateMatrixClientInfoUseCase.execute(aSession)
        }
        coVerify {
            fakeUpdateNotificationSettingsAccountDataUseCase.execute(aSession)
        }
    }

    @Test
    fun `given a session and no start sync needed when execute then it should be configured properly`() = runTest {
        // Given
        val aSession = givenASession()
        every { aSession.coroutineScope } returns this
        fakeWebRtcCallManager.givenCheckForProtocolsSupportIfNeededSucceeds()
        coJustRun { fakeUpdateMatrixClientInfoUseCase.execute(any()) }
        coJustRun { fakeUpdateNotificationSettingsAccountDataUseCase.execute(any()) }
        fakeProgressivePreferences.givenIsClientInfoRecordingEnabled(isEnabled = true)
        fakeNotificationsSettingUpdater.givenOnSessionStarted(aSession)
        fakePushRulesUpdater.givenOnSessionStarted(aSession)

        // When
        configureAndStartSessionUseCase.execute(aSession, startSyncing = false)
        advanceUntilIdle()

        // Then
        verify(inverse = true) { aSession.startSyncing(fakeContext.instance) }
        aSession.fakePushersService.verifyRefreshPushers()
        fakeWebRtcCallManager.verifyCheckForProtocolsSupportIfNeeded()
        coVerify {
            fakeUpdateMatrixClientInfoUseCase.execute(aSession)
            fakeUpdateNotificationSettingsAccountDataUseCase.execute(aSession)
        }
    }

    private fun givenASession(): FakeSession {
        val fakeSession = FakeSession()
        every { fakeSession.open() } just runs
        every { fakeSession.startSyncing(any()) } just runs
        fakeSession.fakePushersService.givenRefreshPushersSucceeds()
        return fakeSession
    }
}
