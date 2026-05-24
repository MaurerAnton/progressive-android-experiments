/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.core.di

import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.progressive.app.features.DefaultProgressiveFeatures
import chat.progressive.app.features.DefaultVectorOverrides
import chat.progressive.app.features.ProgressiveFeatures
import chat.progressive.app.features.VectorOverrides

@InstallIn(SingletonComponent::class)
@Module
object FeaturesModule {

    @Provides
    fun providesFeatures(): ProgressiveFeatures {
        return DefaultProgressiveFeatures()
    }

    @Provides
    fun providesOverrides(): VectorOverrides {
        return DefaultVectorOverrides()
    }
}
