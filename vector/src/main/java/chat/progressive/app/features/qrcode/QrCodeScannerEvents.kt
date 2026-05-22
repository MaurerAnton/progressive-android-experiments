/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.qrcode

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class QrCodeScannerEvents : ProgressiveViewEvents {
    data class CodeParsed(val result: String, val isQrCode: Boolean) : QrCodeScannerEvents()
    object ParseFailed : QrCodeScannerEvents()
    object SwitchMode : QrCodeScannerEvents()
}
