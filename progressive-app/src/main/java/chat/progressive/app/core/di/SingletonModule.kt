/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.core.di

import android.app.Application
import android.content.Context
import android.content.Context.MODE_PRIVATE
import android.content.SharedPreferences
import android.content.res.Resources
import androidx.preference.PreferenceManager
import com.google.i18n.phonenumbers.PhoneNumberUtil
import dagger.Binds
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import chat.progressive.app.EmojiCompatWrapper
import chat.progressive.app.EmojiSpanify
import chat.progressive.app.SpaceStateHandler
import chat.progressive.app.SpaceStateHandlerImpl
import chat.progressive.app.config.Config
import chat.progressive.app.core.device.DefaultGetDeviceInfoUseCase
import chat.progressive.app.core.device.GetDeviceInfoUseCase
import chat.progressive.app.core.dispatchers.CoroutineDispatchers
import chat.progressive.app.core.error.DefaultErrorFormatter
import chat.progressive.app.core.error.ErrorFormatter
import chat.progressive.app.core.resources.BuildMeta
import chat.progressive.app.core.utils.AndroidSystemSettingsProvider
import chat.progressive.app.core.utils.SystemSettingsProvider
import chat.progressive.app.features.analytics.AnalyticsTracker
import chat.progressive.app.features.analytics.ProgressiveAnalytics
import chat.progressive.app.features.analytics.errors.ErrorTracker
import chat.progressive.app.features.analytics.impl.DefaultProgressiveAnalytics
import chat.progressive.app.features.analytics.metrics.ProgressivePlugins
import chat.progressive.app.features.configuration.ProgressiveEventTypes
import chat.progressive.app.features.invite.AutoAcceptInvites
import chat.progressive.app.features.invite.CompileTimeAutoAcceptInvites
import chat.progressive.app.features.mdm.DefaultMdmService
import chat.progressive.app.features.mdm.MdmData
import chat.progressive.app.features.mdm.MdmService
import chat.progressive.app.features.navigation.DefaultNavigator
import chat.progressive.app.features.navigation.Navigator
import chat.progressive.app.features.pin.PinCodeStore
import chat.progressive.app.features.pin.SharedPrefPinCodeStore
import chat.progressive.app.features.room.ProgressiveRoomDisplayName
import chat.progressive.app.features.settings.FontScalePreferences
import chat.progressive.app.features.settings.FontScalePreferencesImpl
import chat.progressive.app.features.settings.ProgressivePreferences
import chat.progressive.app.features.ui.SharedPreferencesUiStateRepository
import chat.progressive.app.features.ui.UiStateRepository
import chat.progressive.application.BuildConfig
import chat.progressive.application.R
import chat.progressive.lib.core.utils.timer.Clock
import chat.progressive.lib.core.utils.timer.DefaultClock
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.SupervisorJob
import org.matrix.android.sdk.api.Matrix
import org.matrix.android.sdk.api.MatrixConfiguration
import org.matrix.android.sdk.api.SyncConfig
import org.matrix.android.sdk.api.auth.AuthenticationService
import org.matrix.android.sdk.api.auth.HomeServerHistoryService
import org.matrix.android.sdk.api.raw.RawService
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.sync.filter.SyncFilterParams
import org.matrix.android.sdk.api.settings.LightweightSettingsStorage
import javax.inject.Singleton

@InstallIn(SingletonComponent::class) @Module abstract class ProgressiveBindModule {

    @Binds
    abstract fun bindNavigator(navigator: DefaultNavigator): Navigator

    @Binds
    abstract fun bindVectorAnalytics(analytics: DefaultProgressiveAnalytics): ProgressiveAnalytics

    @Binds
    abstract fun bindErrorTracker(analytics: DefaultProgressiveAnalytics): ErrorTracker

    @Binds
    abstract fun bindAnalyticsTracker(analytics: DefaultProgressiveAnalytics): AnalyticsTracker

    @Binds
    abstract fun bindErrorFormatter(formatter: DefaultErrorFormatter): ErrorFormatter

    @Binds
    abstract fun bindUiStateRepository(repository: SharedPreferencesUiStateRepository): UiStateRepository

    @Binds
    abstract fun bindPinCodeStore(store: SharedPrefPinCodeStore): PinCodeStore

    @Binds
    abstract fun bindAutoAcceptInvites(autoAcceptInvites: CompileTimeAutoAcceptInvites): AutoAcceptInvites

    @Binds
    abstract fun bindEmojiSpanify(emojiCompatWrapper: EmojiCompatWrapper): EmojiSpanify

    @Binds
    abstract fun bindMdmService(service: DefaultMdmService): MdmService

    @Binds
    abstract fun bindFontScale(fontScale: FontScalePreferencesImpl): FontScalePreferences

    @Binds
    abstract fun bindSystemSettingsProvide(provider: AndroidSystemSettingsProvider): SystemSettingsProvider

    @Binds
    abstract fun bindSpaceStateHandler(spaceStateHandlerImpl: SpaceStateHandlerImpl): SpaceStateHandler

    @Binds
    abstract fun bindGetDeviceInfoUseCase(getDeviceInfoUseCase: DefaultGetDeviceInfoUseCase): GetDeviceInfoUseCase
}

@InstallIn(SingletonComponent::class) @Module object ProgressiveStaticModule {

    @Provides
    fun providesContext(application: Application): Context {
        return application.applicationContext
    }

    @Provides
    fun providesResources(context: Context): Resources {
        return context.resources
    }

    @Provides
    fun providesSharedPreferences(context: Context): SharedPreferences {
        return context.getSharedPreferences("chat.progressive.riot", MODE_PRIVATE)
    }

    @Provides
    fun providesMatrixConfiguration(
            vectorPreferences: ProgressivePreferences,
            progressiveRoomDisplayName: ProgressiveRoomDisplayName,
            vectorPlugins: ProgressivePlugins,
            progressiveEventTypes: ProgressiveEventTypes,
            mdmService: MdmService,
    ): MatrixConfiguration {
        return MatrixConfiguration(
                applicationFlavor = BuildConfig.FLAVOR_DESCRIPTION,
                roomDisplayNameFallbackProvider = progressiveRoomDisplayName,
                threadMessagesEnabledDefault = vectorPreferences.areThreadMessagesEnabled(),
                networkInterceptors = emptyList(),
                metricPlugins = vectorPlugins.plugins(),
                cryptoAnalyticsPlugin = vectorPlugins.cryptoMetricPlugin,
                customEventTypesProvider = progressiveEventTypes,
                clientPermalinkBaseUrl = mdmService.getData(MdmData.PermalinkBaseUrl),
                syncConfig = SyncConfig(
                        syncFilterParams = SyncFilterParams(lazyLoadMembersForStateEvents = true, useThreadNotifications = true)
                )
        )
    }

    @Provides
    @Singleton
    fun providesMatrix(context: Context, configuration: MatrixConfiguration): Matrix {
        return Matrix(context, configuration)
    }

    @Provides
    fun providesCurrentSession(activeSessionHolder: ActiveSessionHolder): Session {
        // TODO handle session injection better
        return activeSessionHolder.getActiveSession()
    }

    @Provides
    fun providesAuthenticationService(matrix: Matrix): AuthenticationService {
        return matrix.authenticationService()
    }

    @Provides
    fun providesRawService(matrix: Matrix): RawService {
        return matrix.rawService()
    }

    @Provides
    fun providesLightweightSettingsStorage(matrix: Matrix): LightweightSettingsStorage {
        return matrix.lightweightSettingsStorage()
    }

    @Provides
    fun providesHomeServerHistoryService(matrix: Matrix): HomeServerHistoryService {
        return matrix.homeServerHistoryService()
    }

    @Provides
    @Singleton
    fun providesApplicationCoroutineScope(): CoroutineScope {
        return CoroutineScope(SupervisorJob() + Dispatchers.Main)
    }

    @Provides
    fun providesCoroutineDispatchers(): CoroutineDispatchers {
        return CoroutineDispatchers(io = Dispatchers.IO, computation = Dispatchers.Default)
    }

    @OptIn(DelicateCoroutinesApi::class)
    @Provides
    @NamedGlobalScope
    fun providesGlobalScope(): CoroutineScope {
        return GlobalScope
    }

    @Provides
    fun providesPhoneNumberUtil(): PhoneNumberUtil = PhoneNumberUtil.getInstance()

    @Provides
    @Singleton
    fun providesBuildMeta(context: Context) = BuildMeta(
            isDebug = BuildConfig.DEBUG,
            applicationId = BuildConfig.APPLICATION_ID,
            applicationName = context.getString(R.string.app_name),
            lowPrivacyLoggingEnabled = Config.LOW_PRIVACY_LOG_ENABLE,
            versionName = BuildConfig.VERSION_NAME,
            gitRevision = BuildConfig.GIT_REVISION,
            gitRevisionDate = BuildConfig.GIT_REVISION_DATE,
            gitBranchName = BuildConfig.GIT_BRANCH_NAME,
            flavorDescription = BuildConfig.FLAVOR_DESCRIPTION,
            flavorShortDescription = BuildConfig.SHORT_FLAVOR_DESCRIPTION,
    )

    @Provides
    @Singleton
    @DefaultPreferences
    fun providesDefaultSharedPreferences(context: Context): SharedPreferences {
        return PreferenceManager.getDefaultSharedPreferences(context.applicationContext)
    }

    @Singleton
    @Provides
    fun providesDefaultClock(): Clock = DefaultClock()
}
