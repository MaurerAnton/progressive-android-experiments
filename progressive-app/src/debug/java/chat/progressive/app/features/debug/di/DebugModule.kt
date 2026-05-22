/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.debug.di

import android.content.Context
import android.content.Intent
import dagger.Binds
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.progressive.app.core.debug.DebugNavigator
import chat.progressive.app.core.debug.DebugReceiver
import chat.progressive.app.core.debug.LeakDetector
import chat.progressive.app.features.debug.DebugMenuActivity
import chat.progressive.app.leakcanary.LeakCanaryLeakDetector
import chat.progressive.app.receivers.VectorDebugReceiver

@InstallIn(SingletonComponent::class)
@Module
abstract class DebugModule {

    companion object {

        @Provides
        fun providesDebugNavigator() = object : DebugNavigator {
            override fun openDebugMenu(context: Context) {
                context.startActivity(Intent(context, DebugMenuActivity::class.java))
            }
        }
    }

    @Binds
    abstract fun bindsDebugReceiver(receiver: VectorDebugReceiver): DebugReceiver

    @Binds
    abstract fun bindsLeakDetector(leakDetector: LeakCanaryLeakDetector): LeakDetector
}
