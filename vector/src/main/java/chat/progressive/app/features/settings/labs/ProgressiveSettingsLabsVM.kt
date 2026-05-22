/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.labs

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.progressive.app.core.di.ActiveSessionHolder
import chat.progressive.app.core.di.MavericksAssistedViewModelFactory
import chat.progressive.app.core.di.hiltMavericksViewModelFactory
import chat.progressive.app.core.platform.EmptyViewEvents
import chat.progressive.app.core.platform.ProgressiveViewModel
import chat.progressive.app.core.session.clientinfo.DeleteMatrixClientInfoUseCase
import chat.progressive.app.core.session.clientinfo.UpdateMatrixClientInfoUseCase
import kotlinx.coroutines.launch

class ProgressiveSettingsLabsVM @AssistedInject constructor(
        @Assisted initialState: ProgressiveSettingsLabsState,
        private val activeSessionHolder: ActiveSessionHolder,
        private val updateMatrixClientInfoUseCase: UpdateMatrixClientInfoUseCase,
        private val deleteMatrixClientInfoUseCase: DeleteMatrixClientInfoUseCase,
) : ProgressiveViewModel<ProgressiveSettingsLabsState, ProgressiveSettingsLabsAction, EmptyViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<ProgressiveSettingsLabsVM, ProgressiveSettingsLabsState> {
        override fun create(initialState: ProgressiveSettingsLabsState): ProgressiveSettingsLabsVM
    }

    companion object : MavericksViewModelFactory<ProgressiveSettingsLabsVM, ProgressiveSettingsLabsState> by hiltMavericksViewModelFactory()

    override fun handle(action: ProgressiveSettingsLabsAction) {
        when (action) {
            ProgressiveSettingsLabsAction.UpdateClientInfo -> handleUpdateClientInfo()
            ProgressiveSettingsLabsAction.DeleteRecordedClientInfo -> handleDeleteRecordedClientInfo()
        }
    }

    private fun handleUpdateClientInfo() {
        viewModelScope.launch {
            activeSessionHolder.getSafeActiveSession()
                    ?.let { session ->
                        updateMatrixClientInfoUseCase.execute(session)
                    }
        }
    }

    private fun handleDeleteRecordedClientInfo() {
        viewModelScope.launch {
            deleteMatrixClientInfoUseCase.execute()
        }
    }
}
