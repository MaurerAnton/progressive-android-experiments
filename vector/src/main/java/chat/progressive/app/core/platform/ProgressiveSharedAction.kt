/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.core.platform

import androidx.lifecycle.ViewModel
import chat.progressive.app.core.utils.MutableDataSource
import chat.progressive.app.core.utils.PublishDataSource

interface ProgressiveSharedAction

/**
 * Parent class to handle navigation events, action events, or other any events.
 */
open class ProgressiveSharedAction<T : ProgressiveSharedAction>(private val store: MutableDataSource<T> = PublishDataSource()) :
        ViewModel(), MutableDataSource<T> by store
