/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.spaces.create

import chat.progressive.app.core.platform.ProgressiveViewEvents

sealed class CreateSpaceEvents : ProgressiveViewEvents {
    object NavigateToDetails : CreateSpaceEvents()
    object NavigateToChooseType : CreateSpaceEvents()
    object NavigateToAddRooms : CreateSpaceEvents()
    object NavigateToAdd3Pid : CreateSpaceEvents()
    object NavigateToChoosePrivateType : CreateSpaceEvents()
    object Dismiss : CreateSpaceEvents()
    data class FinishSuccess(val spaceId: String, val defaultRoomId: String?, val topology: SpaceTopology?) : CreateSpaceEvents()
    data class ShowModalError(val errorMessage: String) : CreateSpaceEvents()
    object HideModalLoading : CreateSpaceEvents()
    data class ShowModalLoading(val message: String?) : CreateSpaceEvents()
}
