/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.di

import dagger.Binds
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.progressive.app.GoogleFlavorLegals
import chat.progressive.app.core.pushers.FcmHelper
import chat.progressive.app.core.resources.AppNameProvider
import chat.progressive.app.core.resources.DefaultAppNameProvider
import chat.progressive.app.core.resources.DefaultLocaleProvider
import chat.progressive.app.core.resources.LocaleProvider
import chat.progressive.app.core.services.GuardServiceStarter
import chat.progressive.app.features.home.NightlyProxy
import chat.progressive.app.features.settings.legals.FlavorLegals
import chat.progressive.app.nightly.FirebaseNightlyProxy
import chat.progressive.app.push.fcm.GoogleFcmHelper

@InstallIn(SingletonComponent::class)
@Module
abstract class FlavorModule {

    companion object {
        @Provides
        fun provideGuardServiceStarter(): GuardServiceStarter {
            return object : GuardServiceStarter {}
        }
    }

    @Binds
    abstract fun bindsNightlyProxy(nightlyProxy: FirebaseNightlyProxy): NightlyProxy

    @Binds
    abstract fun bindsFcmHelper(fcmHelper: GoogleFcmHelper): FcmHelper

    @Binds
    abstract fun bindsLocaleProvider(localeProvider: DefaultLocaleProvider): LocaleProvider

    @Binds
    abstract fun bindsAppNameProvider(appNameProvider: DefaultAppNameProvider): AppNameProvider

    @Binds
    abstract fun bindsFlavorLegals(legals: GoogleFlavorLegals): FlavorLegals
}
