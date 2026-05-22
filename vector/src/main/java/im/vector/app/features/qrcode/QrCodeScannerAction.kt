/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.qrcode

import im.vector.app.core.platform.ProgressiveViewModelAction

sealed class QrCodeScannerAction : ProgressiveViewModelAction {
    data class CodeDecoded(
            val result: String,
            val isQrCode: Boolean
    ) : QrCodeScannerAction()

    object ScanFailed : QrCodeScannerAction()

    object SwitchMode : QrCodeScannerAction()
}
