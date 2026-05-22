/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.call

import chat.progressive.app.core.platform.ProgressiveViewEvents
import chat.progressive.app.features.call.audio.CallAudioManager
import org.matrix.android.sdk.api.session.call.TurnServerResponse

sealed class ProgressiveCallViewEvents : ProgressiveViewEvents {

    data class ConnectionTimeout(val turn: TurnServerResponse?) : ProgressiveCallViewEvents()
    data class ShowSoundDeviceChooser(
            val available: Set<CallAudioManager.Device>,
            val current: CallAudioManager.Device
    ) : ProgressiveCallViewEvents()

    object ShowDialPad : ProgressiveCallViewEvents()
    object ShowCallTransferScreen : ProgressiveCallViewEvents()
    object FailToTransfer : ProgressiveCallViewEvents()
    object ShowScreenSharingPermissionDialog : ProgressiveCallViewEvents()
    object StopScreenSharingService : ProgressiveCallViewEvents()
}
