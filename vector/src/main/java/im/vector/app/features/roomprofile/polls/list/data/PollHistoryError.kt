/*
 * Copyright 2023, 2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.features.roomprofile.polls.list.data

sealed class PollHistoryError : Exception() {
    object UnknownRoomError : PollHistoryError()
}
