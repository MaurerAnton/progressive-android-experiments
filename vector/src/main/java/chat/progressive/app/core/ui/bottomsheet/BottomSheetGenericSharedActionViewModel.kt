/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package chat.progressive.app.core.ui.bottomsheet

import chat.progressive.app.core.platform.ProgressiveSharedAction
import chat.progressive.app.core.platform.ProgressiveSharedAction

/**
 * Activity shared view model to handle bottom sheet quick actions.
 */
abstract class BottomSheetGenericSharedActionViewModel<Action : ProgressiveSharedAction> : ProgressiveSharedAction<Action>()
