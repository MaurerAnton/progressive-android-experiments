/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package chat.progressive.app.features.roomprofile.alias.detail

import chat.progressive.app.core.platform.ProgressiveSharedAction
import chat.progressive.app.core.platform.ProgressiveSharedActionViewModel
import javax.inject.Inject

/**
 * Activity shared view model to handle room alias quick actions.
 */
class RoomAliasBottomSheetSharedActionViewModel @Inject constructor() : ProgressiveSharedActionViewModel<RoomAliasBottomSheetSharedAction>()
