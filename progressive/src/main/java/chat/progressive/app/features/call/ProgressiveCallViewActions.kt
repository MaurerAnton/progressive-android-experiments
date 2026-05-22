/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.call

import chat.progressive.app.core.platform.ProgressiveViewModelAction
import chat.progressive.app.features.call.audio.CallAudioManager
import chat.progressive.app.features.call.transfer.CallTransferResult
import org.webrtc.VideoCapturer

sealed class ProgressiveCallViewActions : ProgressiveViewModelAction {
    object EndCall : ProgressiveCallViewActions()
    object AcceptCall : ProgressiveCallViewActions()
    object DeclineCall : ProgressiveCallViewActions()
    object ToggleMute : ProgressiveCallViewActions()
    object ToggleVideo : ProgressiveCallViewActions()
    object ToggleHoldResume : ProgressiveCallViewActions()
    data class ChangeAudioDevice(val device: CallAudioManager.Device) : ProgressiveCallViewActions()
    object OpenDialPad : ProgressiveCallViewActions()
    data class SendDtmfDigit(val digit: String) : ProgressiveCallViewActions()
    data class SwitchCall(val callArgs: CallArgs) : ProgressiveCallViewActions()

    object SwitchSoundDevice : ProgressiveCallViewActions()
    object HeadSetButtonPressed : ProgressiveCallViewActions()
    object ToggleCamera : ProgressiveCallViewActions()
    object ToggleHDSD : ProgressiveCallViewActions()
    object InitiateCallTransfer : ProgressiveCallViewActions()
    object CallTransferSelectionCancelled : ProgressiveCallViewActions()
    data class CallTransferSelectionResult(val callTransferResult: CallTransferResult) : ProgressiveCallViewActions()
    object TransferCall : ProgressiveCallViewActions()
    object ToggleScreenSharing : ProgressiveCallViewActions()
    data class StartScreenSharing(val videoCapturer: VideoCapturer) : ProgressiveCallViewActions()
}
