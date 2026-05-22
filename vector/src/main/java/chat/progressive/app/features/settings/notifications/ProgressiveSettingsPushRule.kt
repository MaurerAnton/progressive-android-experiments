/*
 * Copyright 2023, 2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.notifications

import android.os.Bundle
import android.view.View
import androidx.preference.Preference
import com.airbnb.mvrx.fragmentViewModel
import com.airbnb.mvrx.withState
import chat.progressive.app.core.preference.ProgressiveCheckboxPreference
import chat.progressive.app.features.settings.ProgressiveSettingsBaseFragment
import chat.progressive.app.features.themes.ThemeUtils
import chat.progressive.lib.strings.CommonStrings

abstract class ProgressiveSettingsPushRule :
        ProgressiveSettingsBaseFragment() {

    protected val viewModel: ProgressiveSettingsPushRuleVM by fragmentViewModel()

    abstract val prefKeyToPushRuleId: Map<String, String>

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        observeViewEvents()
        viewModel.onEach(ProgressiveSettingsPushRuleState::allRules) { refreshPreferences() }
        viewModel.onEach(ProgressiveSettingsPushRuleState::isLoading) { updateLoadingView(it) }
        viewModel.onEach(ProgressiveSettingsPushRuleState::rulesOnError) { refreshErrors(it) }
    }

    private fun observeViewEvents() {
        viewModel.observeViewEvents {
            when (it) {
                is ProgressiveSettingsPushRuleEvent.Failure -> onFailure(it.ruleId)
                is ProgressiveSettingsPushRuleEvent.PushRuleUpdated -> {
                    updatePreference(it.ruleId, it.checked)
                    if (it.failure != null) {
                        onFailure(it.ruleId)
                    }
                }
            }
        }
    }

    override fun bindPref() {
        prefKeyToPushRuleId.forEach { (preferenceKey, ruleId) ->
            findPreference<ProgressiveCheckboxPreference>(preferenceKey)?.apply {
                isIconSpaceReserved = false
                onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
                    viewModel.handle(ProgressiveSettingsPushRuleAction.UpdatePushRule(ruleId, newValue as Boolean))
                    false
                }
            }
        }
    }

    private fun updateLoadingView(isLoading: Boolean) {
        if (isLoading) {
            displayLoadingView()
        } else {
            hideLoadingView()
        }
    }

    private fun refreshPreferences() {
        prefKeyToPushRuleId.values.forEach { ruleId -> updatePreference(ruleId, viewModel.isPushRuleChecked(ruleId)) }
    }

    private fun refreshErrors(rulesWithError: Set<String>) {
        if (withState(viewModel, ProgressiveSettingsPushRuleState::isLoading)) return
        prefKeyToPushRuleId.forEach { (preferenceKey, ruleId) ->
            findPreference<ProgressiveCheckboxPreference>(preferenceKey)?.apply {
                if (ruleId in rulesWithError) {
                    summaryTextColor = ThemeUtils.getColor(context, com.google.android.material.R.attr.colorError)
                    setSummary(CommonStrings.settings_notification_error_on_update)
                } else {
                    summaryTextColor = null
                    summary = null
                }
            }
        }
    }

    protected fun refreshDisplay() {
        listView?.adapter?.notifyDataSetChanged()
    }

    private fun updatePreference(ruleId: String, checked: Boolean) {
        val preferenceKey = prefKeyToPushRuleId.entries.find { it.value == ruleId }?.key ?: return
        val preference = findPreference<ProgressiveCheckboxPreference>(preferenceKey) ?: return
        val ruleIds = withState(viewModel) { state -> state.allRules.map { it.ruleId } }
        preference.isVisible = ruleId in ruleIds
        preference.isChecked = checked
    }

    protected open fun onFailure(ruleId: String) {
        refreshDisplay()
    }
}
