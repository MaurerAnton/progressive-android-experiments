/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.attachments

import com.airbnb.mvrx.MavericksState
import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import chat.progressive.app.core.di.MavericksAssistedViewModelFactory
import chat.progressive.app.core.di.hiltMavericksViewModelFactory
import chat.progressive.app.core.platform.EmptyViewEvents
import chat.progressive.app.core.platform.ProgressiveViewModel
import chat.progressive.app.core.platform.ProgressiveViewModelAction
import chat.progressive.app.features.ProgressiveFeatures
import chat.progressive.app.features.settings.ProgressiveBasePreferences

class AttachmentTypeSelectorViewModel @AssistedInject constructor(
        @Assisted initialState: AttachmentTypeSelectorViewState,
        private val vectorFeatures: ProgressiveFeatures,
        private val progressivePreferences: ProgressiveBasePreferences,
) : ProgressiveViewModel<AttachmentTypeSelectorViewState, AttachmentTypeSelectorAction, EmptyViewEvents>(initialState) {
    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<AttachmentTypeSelectorViewModel, AttachmentTypeSelectorViewState> {
        override fun create(initialState: AttachmentTypeSelectorViewState): AttachmentTypeSelectorViewModel
    }

    companion object : MavericksViewModelFactory<AttachmentTypeSelectorViewModel, AttachmentTypeSelectorViewState> by hiltMavericksViewModelFactory()

    override fun handle(action: AttachmentTypeSelectorAction) = when (action) {
        is AttachmentTypeSelectorAction.ToggleTextFormatting -> setTextFormattingEnabled(action.isEnabled)
    }

    init {
        setState {
            copy(
                    isLocationVisible = vectorFeatures.isLocationSharingEnabled(),
                    isVoiceBroadcastVisible = vectorFeatures.isVoiceBroadcastEnabled(),
                    isTextFormattingEnabled = progressivePreferences.isTextFormattingEnabled(),
            )
        }
    }

    private fun setTextFormattingEnabled(isEnabled: Boolean) {
        progressivePreferences.setTextFormattingEnabled(isEnabled)
        setState {
            copy(
                    isTextFormattingEnabled = isEnabled
            )
        }
    }
}

data class AttachmentTypeSelectorViewState(
        val isLocationVisible: Boolean = false,
        val isVoiceBroadcastVisible: Boolean = false,
        val isTextFormattingEnabled: Boolean = false,
) : MavericksState

sealed interface AttachmentTypeSelectorAction : ProgressiveViewModelAction {
    data class ToggleTextFormatting(val isEnabled: Boolean) : AttachmentTypeSelectorAction
}
