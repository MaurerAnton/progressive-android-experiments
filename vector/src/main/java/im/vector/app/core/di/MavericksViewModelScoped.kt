/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.core.di

import javax.inject.Scope

/**
 * Scope annotation for bindings that should exist for the life of an MavericksViewModel.
 */
@Scope
annotation class MavericksViewModelScoped
