/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.analytics.ui.consent

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.progressive.app.core.di.MavericksAssistedViewModelFactory
import chat.progressive.app.core.di.hiltMavericksViewModelFactory
import chat.progressive.app.core.platform.ProgressiveViewModel
import chat.progressive.app.features.analytics.ProgressiveAnalytics
import kotlinx.coroutines.launch

class AnalyticsConsentViewModel @AssistedInject constructor(
        @Assisted initialState: AnalyticsConsentViewState,
        private val analytics: ProgressiveAnalytics
) : ProgressiveViewModel<AnalyticsConsentViewState, AnalyticsConsentViewActions, AnalyticsOptInViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<AnalyticsConsentViewModel, AnalyticsConsentViewState> {
        override fun create(initialState: AnalyticsConsentViewState): AnalyticsConsentViewModel
    }

    companion object : MavericksViewModelFactory<AnalyticsConsentViewModel, AnalyticsConsentViewState> by hiltMavericksViewModelFactory()

    init {
        observeAnalytics()
    }

    private fun observeAnalytics() {
        analytics.didAskUserConsent().setOnEach {
            copy(didAskUserConsent = it)
        }
        analytics.getUserConsent().setOnEach {
            copy(userConsent = it)
        }
    }

    override fun handle(action: AnalyticsConsentViewActions) {
        when (action) {
            is AnalyticsConsentViewActions.SetUserConsent -> handleSetUserConsent(action)
        }
    }

    private fun handleSetUserConsent(action: AnalyticsConsentViewActions.SetUserConsent) {
        viewModelScope.launch {
            analytics.setUserConsent(action.userConsent)
            analytics.setDidAskUserConsent()
            _viewEvents.post(AnalyticsOptInViewEvents.OnDataSaved)
        }
    }
}
