/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings

import android.os.Bundle
import androidx.preference.Preference
import androidx.preference.SeekBarPreference
import dagger.hilt.android.AndroidEntryPoint
import chat.progressive.app.R
import chat.progressive.app.core.platform.ProgressiveActivity
import chat.progressive.app.core.preference.ProgressiveBasePreference
import chat.progressive.app.core.preference.ProgressivePreferenceCategory
import chat.progressive.app.core.preference.ProgressiveSwitchPreference
import chat.progressive.app.core.utils.copyToClipboard
import chat.progressive.app.features.analytics.plan.MobileScreen
import chat.progressive.app.features.home.NightlyProxy
import chat.progressive.app.features.rageshake.RageShake
import chat.progressive.lib.strings.CommonStrings
import javax.inject.Inject

@AndroidEntryPoint
class ProgressiveSettingsAdvancedSettingsFragment :
        ProgressiveSettingsBaseFragment() {

    override var titleRes = CommonStrings.settings_advanced_settings
    override val preferenceXmlRes = R.xml.progressive_settings_advanced_settings

    @Inject lateinit var nightlyProxy: NightlyProxy

    private var rageshake: RageShake? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsAdvanced
    }

    override fun onResume() {
        super.onResume()

        rageshake = (activity as? ProgressiveActivity<*>)?.rageShake
        rageshake?.interceptor = {
            (activity as? ProgressiveActivity<*>)?.showSnackbar(getString(CommonStrings.rageshake_detected))
        }
    }

    override fun onPause() {
        super.onPause()
        rageshake?.interceptor = null
        rageshake = null
    }

    override fun bindPref() {
        setupRageShakeSection()
        setupNightlySection()
        setupDevToolsSection()
    }

    private fun setupDevToolsSection() {
        findPreference<ProgressiveBasePreference>("SETTINGS_ACCESS_TOKEN")?.setOnPreferenceClickListener {
            copyToClipboard(requireActivity(), session.sessionParams.credentials.accessToken)
            true
        }

        findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_DEVELOPER_MODE_KEY_REQUEST_AUDIT_KEY)?.apply {
            isVisible = session.cryptoService().supportKeyRequestInspection()
        }
    }

    private fun setupRageShakeSection() {
        val isRageShakeAvailable = RageShake.isAvailable(requireContext())

        if (isRageShakeAvailable) {
            findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_USE_RAGE_SHAKE_KEY)!!
                    .onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->

                if (newValue as? Boolean == true) {
                    rageshake?.start()
                } else {
                    rageshake?.stop()
                }

                true
            }

            findPreference<SeekBarPreference>(ProgressiveBasePreferences.SETTINGS_RAGE_SHAKE_DETECTION_THRESHOLD_KEY)!!
                    .onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
                (activity as? ProgressiveActivity<*>)?.let {
                    val newValueAsInt = newValue as? Int ?: return@OnPreferenceChangeListener true

                    rageshake?.setSensitivity(newValueAsInt)
                }

                true
            }
        } else {
            findPreference<ProgressivePreferenceCategory>("SETTINGS_RAGE_SHAKE_CATEGORY_KEY")!!.isVisible = false
        }
    }

    private fun setupNightlySection() {
        findPreference<ProgressivePreferenceCategory>("SETTINGS_NIGHTLY_BUILD_PREFERENCE_KEY")?.isVisible = nightlyProxy.isNightlyBuild()
        findPreference<ProgressiveBasePreference>("SETTINGS_NIGHTLY_BUILD_UPDATE_PREFERENCE_KEY")?.setOnPreferenceClickListener {
            nightlyProxy.updateApplication()
            true
        }
    }
}
