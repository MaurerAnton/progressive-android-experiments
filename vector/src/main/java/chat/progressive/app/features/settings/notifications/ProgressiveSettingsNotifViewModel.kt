/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.notifications

import android.content.SharedPreferences
import androidx.annotation.VisibleForTesting
import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.progressive.app.core.di.MavericksAssistedViewModelFactory
import chat.progressive.app.core.di.hiltMavericksViewModelFactory
import chat.progressive.app.core.platform.ProgressiveDummyViewState
import chat.progressive.app.core.platform.ProgressiveViewModel
import chat.progressive.app.core.pushers.EnsureFcmTokenIsRetrievedUseCase
import chat.progressive.app.core.pushers.PushersManager
import chat.progressive.app.core.pushers.RegisterUnifiedPushUseCase
import chat.progressive.app.core.pushers.UnregisterUnifiedPushUseCase
import chat.progressive.app.features.settings.ProgressiveBasePreferences
import chat.progressive.app.features.settings.notifications.usecase.DisableNotificationsForCurrentSessionUseCase
import chat.progressive.app.features.settings.notifications.usecase.EnableNotificationsForCurrentSessionUseCase
import chat.progressive.app.features.settings.notifications.usecase.ToggleNotificationsForCurrentSessionUseCase
import kotlinx.coroutines.launch

class ProgressiveSettingsNotifViewModel @AssistedInject constructor(
        @Assisted initialState: ProgressiveDummyViewState,
        private val pushersManager: PushersManager,
        private val progressivePreferences: ProgressiveBasePreferences,
        private val enableNotificationsForCurrentSessionUseCase: EnableNotificationsForCurrentSessionUseCase,
        private val disableNotificationsForCurrentSessionUseCase: DisableNotificationsForCurrentSessionUseCase,
        private val unregisterUnifiedPushUseCase: UnregisterUnifiedPushUseCase,
        private val registerUnifiedPushUseCase: RegisterUnifiedPushUseCase,
        private val ensureFcmTokenIsRetrievedUseCase: EnsureFcmTokenIsRetrievedUseCase,
        private val toggleNotificationsForCurrentSessionUseCase: ToggleNotificationsForCurrentSessionUseCase,
) : ProgressiveViewModel<ProgressiveDummyViewState, ProgressiveSettingsNotifAction, ProgressiveSettingsNotifEvent>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<ProgressiveSettingsNotifViewModel, ProgressiveDummyViewState> {
        override fun create(initialState: ProgressiveDummyViewState): ProgressiveSettingsNotifViewModel
    }

    companion object : MavericksViewModelFactory<ProgressiveSettingsNotifViewModel, ProgressiveDummyViewState> by hiltMavericksViewModelFactory()

    @VisibleForTesting
    val notificationsPreferenceListener: SharedPreferences.OnSharedPreferenceChangeListener =
            SharedPreferences.OnSharedPreferenceChangeListener { _, key ->
                if (key == ProgressiveBasePreferences.SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY) {
                    if (progressivePreferences.areNotificationEnabledForDevice()) {
                        _viewEvents.post(ProgressiveSettingsNotifEvent.NotificationsForDeviceEnabled)
                    } else {
                        _viewEvents.post(ProgressiveSettingsNotifEvent.NotificationsForDeviceDisabled)
                    }
                }
            }

    init {
        observeNotificationsEnabledPreference()
    }

    private fun observeNotificationsEnabledPreference() {
        progressivePreferences.subscribeToChanges(notificationsPreferenceListener)
    }

    override fun onCleared() {
        progressivePreferences.unsubscribeToChanges(notificationsPreferenceListener)
        super.onCleared()
    }

    override fun handle(action: ProgressiveSettingsNotifAction) {
        when (action) {
            ProgressiveSettingsNotifAction.DisableNotificationsForDevice -> handleDisableNotificationsForDevice()
            is ProgressiveSettingsNotifAction.EnableNotificationsForDevice -> handleEnableNotificationsForDevice(action.pushDistributor)
            is ProgressiveSettingsNotifAction.RegisterPushDistributor -> handleRegisterPushDistributor(action.pushDistributor)
        }
    }

    private fun handleDisableNotificationsForDevice() {
        viewModelScope.launch {
            disableNotificationsForCurrentSessionUseCase.execute()
            _viewEvents.post(ProgressiveSettingsNotifEvent.NotificationsForDeviceDisabled)
        }
    }

    private fun handleEnableNotificationsForDevice(distributor: String) {
        viewModelScope.launch {
            when (enableNotificationsForCurrentSessionUseCase.execute(distributor)) {
                is EnableNotificationsForCurrentSessionUseCase.EnableNotificationsResult.NeedToAskUserForDistributor -> {
                    _viewEvents.post(ProgressiveSettingsNotifEvent.AskUserForPushDistributor)
                }
                EnableNotificationsForCurrentSessionUseCase.EnableNotificationsResult.Success -> {
                    _viewEvents.post(ProgressiveSettingsNotifEvent.NotificationsForDeviceEnabled)
                }
            }
        }
    }

    private fun handleRegisterPushDistributor(distributor: String) {
        viewModelScope.launch {
            unregisterUnifiedPushUseCase.execute(pushersManager)
            when (registerUnifiedPushUseCase.execute(distributor)) {
                RegisterUnifiedPushUseCase.RegisterUnifiedPushResult.NeedToAskUserForDistributor -> {
                    _viewEvents.post(ProgressiveSettingsNotifEvent.AskUserForPushDistributor)
                }
                RegisterUnifiedPushUseCase.RegisterUnifiedPushResult.Success -> {
                    val areNotificationsEnabled = progressivePreferences.areNotificationEnabledForDevice()
                    ensureFcmTokenIsRetrievedUseCase.execute(pushersManager, registerPusher = areNotificationsEnabled)
                    toggleNotificationsForCurrentSessionUseCase.execute(enabled = areNotificationsEnabled)
                    _viewEvents.post(ProgressiveSettingsNotifEvent.NotificationMethodChanged)
                }
            }
        }
    }
}
