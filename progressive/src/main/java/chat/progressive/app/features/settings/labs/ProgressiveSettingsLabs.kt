/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.labs

import android.os.Build
import android.os.Bundle
import android.text.method.LinkMovementMethod
import android.widget.TextView
import androidx.preference.Preference
import androidx.preference.Preference.OnPreferenceChangeListener
import androidx.preference.SwitchPreference
import com.airbnb.mvrx.fragmentViewModel
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import dagger.hilt.android.AndroidEntryPoint
import chat.progressive.app.R
import chat.progressive.app.core.preference.ProgressiveSwitchPreference
import chat.progressive.app.features.MainActivity
import chat.progressive.app.features.MainActivityArgs
import chat.progressive.app.features.ProgressiveFeatures
import im.vector.app.features.analytics.plan.MobileScreen
import chat.progressive.app.features.home.room.threads.ThreadsManager
import chat.progressive.app.features.settings.ProgressiveBasePreferences
import chat.progressive.app.features.settings.ProgressiveSettingsBaseFragment
import chat.progressive.lib.strings.CommonStrings
import org.matrix.android.sdk.api.settings.LightweightSettingsStorage
import javax.inject.Inject

@AndroidEntryPoint
class ProgressiveSettingsLabs :
        ProgressiveSettingsBaseFragment() {

    private val viewModel: ProgressiveSettingsLabsVM by fragmentViewModel()

    @Inject lateinit var progressivePreferences: ProgressiveBasePreferences
    @Inject lateinit var lightweightSettingsStorage: LightweightSettingsStorage
    @Inject lateinit var threadsManager: ThreadsManager
    @Inject lateinit var vectorFeatures: ProgressiveFeatures

    override var titleRes = CommonStrings.room_settings_labs_pref_title
    override val preferenceXmlRes = R.xml.progressive_settings_labs

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsLabs
    }

    override fun bindPref() {
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_AUTO_REPORT_UISI)?.let { pref ->
            // ensure correct default
            pref.isChecked = progressivePreferences.labsAutoReportUISI()
        }

        // clear cache
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_ENABLE_THREAD_MESSAGES)?.let { vectorPref ->
            vectorPref.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                onThreadsPreferenceClickedInterceptor(vectorPref)
                false
            }
        }

        findPreference<SwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_MSC3061_SHARE_KEYS_HISTORY)?.let { pref ->
            if (session.cryptoService().supportsShareKeysOnInvite()) {
                // ensure correct default
                pref.isChecked = session.cryptoService().isShareKeysOnInviteEnabled()

                pref.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                    session.cryptoService().enableShareKeyOnInvite(pref.isChecked)
                    MainActivity.restartApp(requireActivity(), MainActivityArgs(clearCache = true))
                    true
                }
            } else {
                pref.isEnabled = false
                pref.isChecked = false
            }
        }

        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_NEW_APP_LAYOUT_KEY)?.let { pref ->
            pref.isVisible = vectorFeatures.isNewAppLayoutFeatureEnabled()

            pref.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                onNewLayoutPreferenceClicked()
                true
            }
        }

        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_VOICE_BROADCAST_KEY)?.let { pref ->
            // Voice Broadcast recording is not available on Android < 10
            pref.isVisible = Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q && vectorFeatures.isVoiceBroadcastEnabled()
        }

        configureUnreadNotificationsAsTabPreference()
        configureEnableClientInfoRecordingPreference()
        configureProgressiveLabsPreferences()
    }

    /**
     * Wire up progressive-specific labs preferences to native C++ handlers.
     */
    private fun configureProgressiveLabsPreferences() {
        // Transparent Overlay
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_TRANSPARENT_OVERLAY_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setTransparentOverlayEnabled(enabled)
                true
            }

        // Text Undo
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_TEXT_UNDO_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setTextUndoEnabled(enabled)
                true
            }

        // LLM Slash Commands
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_LLM_SLASH_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setLlmSlashEnabled(enabled)
                true
            }

        // Web Search
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_WEB_SEARCH_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setWebSearchEnabled(enabled)
                true
            }

        // Native DB backend
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_NATIVE_DB_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setNativeDbEnabled(enabled)
                true
            }

        // Yggdrasil network
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_YGGDRASIL_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setYggdrasilEnabled(enabled)
                true
            }

        // Tor routing
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_TOR_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setTorEnabled(enabled)
                true
            }

        // I2P integration
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_I2P_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setI2pEnabled(enabled)
                true
            }

        // Chat Pushdown
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_CHAT_PUSHDOWN_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setChatPushdownEnabled(enabled)
                true
            }

        // Desync Detector
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_DESYNC_DETECTOR_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setDesyncDetectorEnabled(enabled)
                true
            }

        // Profile Swiper
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_PROFILE_SWIPER_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setProfileSwiperEnabled(enabled)
                true
            }

        // SVG Render
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_SVG_DRAW_KEY)?.onPreferenceChangeListener =
            OnPreferenceChangeListener { _, newValue ->
                val enabled = newValue as? Boolean ?: false
                progressivePreferences.setSvgDrawEnabled(enabled)
                true
            }
    }

    private fun configureUnreadNotificationsAsTabPreference() {
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_UNREAD_NOTIFICATIONS_AS_TAB)?.let { pref ->
            pref.isVisible = !vectorFeatures.isNewAppLayoutFeatureEnabled()
            pref.isEnabled = !progressivePreferences.isNewAppLayoutEnabled()
        }
    }

    /**
     * Intercept the click to display a user friendly dialog when their homeserver do not support threads.
     */
    private fun onThreadsPreferenceClickedInterceptor(vectorSwitchPreference: ProgressiveSwitchPreference) {
        val userEnabledThreads = progressivePreferences.areThreadMessagesEnabled()
        if (!session.homeServerCapabilitiesService().getHomeServerCapabilities().canUseThreading && userEnabledThreads) {
            activity?.let {
                MaterialAlertDialogBuilder(it)
                        .setTitle(CommonStrings.threads_labs_enable_notice_title)
                        .setMessage(threadsManager.getLabsEnableThreadsMessage())
                        .setCancelable(true)
                        .setNegativeButton(CommonStrings.action_not_now) { _, _ ->
                            vectorSwitchPreference.isChecked = false
                        }
                        .setPositiveButton(CommonStrings.action_try_it_out) { _, _ ->
                            onThreadsPreferenceClicked()
                        }
                        .show()
                        ?.findViewById<TextView>(android.R.id.message)
                        ?.apply {
                            linksClickable = true
                            movementMethod = LinkMovementMethod.getInstance()
                        }
            }
        } else {
            onThreadsPreferenceClicked()
        }
    }

    /**
     * Action when threads preference switch is actually clicked.
     */
    private fun onThreadsPreferenceClicked() {
        // We should migrate threads only if threads are disabled
        progressivePreferences.setThreadFlagChangedManually()
        progressivePreferences.setShouldMigrateThreads(!progressivePreferences.areThreadMessagesEnabled())
        lightweightSettingsStorage.setThreadMessagesEnabled(progressivePreferences.areThreadMessagesEnabled())
        displayLoadingView()
        MainActivity.restartApp(requireActivity(), MainActivityArgs(clearCache = true))
    }

    /**
     * Action when new layout preference switch is actually clicked.
     */
    private fun onNewLayoutPreferenceClicked() {
        configureUnreadNotificationsAsTabPreference()
    }

    private fun configureEnableClientInfoRecordingPreference() {
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_LABS_CLIENT_INFO_RECORDING_KEY)?.onPreferenceChangeListener =
                OnPreferenceChangeListener { _, newValue ->
                    when (newValue as? Boolean) {
                        false -> viewModel.handle(ProgressiveSettingsLabsAction.DeleteRecordedClientInfo)
                        true -> viewModel.handle(ProgressiveSettingsLabsAction.UpdateClientInfo)
                        else -> Unit
                    }
                    true
                }
    }
}
