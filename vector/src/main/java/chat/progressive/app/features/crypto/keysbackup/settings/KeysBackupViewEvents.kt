/*
 * Copyright 2022-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.crypto.keysbackup.settings

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class KeysBackupViewEvents : ProgressiveViewEvents {
    object OpenLegacyCreateBackup : KeysBackupViewEvents()
    data class RequestStore4SSecret(val recoveryKey: String) : KeysBackupViewEvents()
}
