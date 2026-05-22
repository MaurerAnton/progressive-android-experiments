/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.qrcode

import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import im.vector.app.core.di.MavericksAssistedViewModelFactory
import im.vector.app.core.di.hiltMavericksViewModelFactory
import im.vector.app.core.platform.ProgressiveDummyViewState
import im.vector.app.core.platform.ProgressiveViewModel

class QrCodeScannerViewModel @AssistedInject constructor(
        @Assisted initialState: ProgressiveDummyViewState,
) : ProgressiveViewModel<ProgressiveDummyViewState, QrCodeScannerAction, QrCodeScannerEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<QrCodeScannerViewModel, ProgressiveDummyViewState> {
        override fun create(initialState: ProgressiveDummyViewState): QrCodeScannerViewModel
    }

    companion object : MavericksViewModelFactory<QrCodeScannerViewModel, ProgressiveDummyViewState> by hiltMavericksViewModelFactory()

    override fun handle(action: QrCodeScannerAction) {
        _viewEvents.post(
                when (action) {
                    is QrCodeScannerAction.CodeDecoded -> QrCodeScannerEvents.CodeParsed(action.result, action.isQrCode)
                    is QrCodeScannerAction.SwitchMode -> QrCodeScannerEvents.SwitchMode
                    is QrCodeScannerAction.ScanFailed -> QrCodeScannerEvents.ParseFailed
                }
        )
    }
}
