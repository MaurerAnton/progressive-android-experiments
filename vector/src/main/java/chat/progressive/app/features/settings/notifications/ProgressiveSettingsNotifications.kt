/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.notifications

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.media.RingtoneManager
import android.net.Uri
import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.lifecycle.LiveData
import androidx.lifecycle.distinctUntilChanged
import androidx.lifecycle.lifecycleScope
import androidx.lifecycle.map
import androidx.preference.Preference
import androidx.preference.SwitchPreference
import com.airbnb.mvrx.fragmentViewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.progressive.app.R
import chat.progressive.app.core.di.ActiveSessionHolder
import chat.progressive.app.core.extensions.registerStartForActivityResult
import chat.progressive.app.core.preference.ProgressiveEditTextPreference
import chat.progressive.app.core.preference.ProgressiveBasePreference
import chat.progressive.app.core.preference.ProgressivePreferenceCategory
import chat.progressive.app.core.preference.ProgressiveSwitchPreference
import chat.progressive.app.core.pushers.EnsureFcmTokenIsRetrievedUseCase
import chat.progressive.app.core.pushers.FcmHelper
import chat.progressive.app.core.pushers.PushersManager
import chat.progressive.app.core.pushers.UnifiedPushHelper
import chat.progressive.app.core.services.GuardServiceStarter
import chat.progressive.app.core.utils.combineLatest
import chat.progressive.app.core.utils.isIgnoringBatteryOptimizations
import chat.progressive.app.core.utils.registerForPermissionsResult
import chat.progressive.app.core.utils.requestDisablingBatteryOptimization
import chat.progressive.app.core.utils.startNotificationSettingsIntent
import chat.progressive.app.features.ProgressiveFeatures
import chat.progressive.app.features.analytics.plan.MobileScreen
import chat.progressive.app.features.home.NotificationPermissionManager
import chat.progressive.app.features.notifications.NotificationUtils
import chat.progressive.app.features.settings.BackgroundSyncMode
import chat.progressive.app.features.settings.BackgroundSyncModeChooserDialog
import chat.progressive.app.features.settings.ProgressiveBasePreferences
import chat.progressive.app.features.settings.ProgressiveSettingsBaseFragment
import chat.progressive.app.features.settings.ProgressiveSettingsInteraction
import chat.progressive.lib.core.utils.compat.getParcelableExtraCompat
import chat.progressive.lib.strings.CommonPlurals
import chat.progressive.lib.strings.CommonStrings
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch
import org.matrix.android.sdk.api.extensions.tryOrNull
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.identity.ThreePid
import org.matrix.android.sdk.api.session.pushers.Pusher
import org.matrix.android.sdk.api.session.pushrules.RuleIds
import org.matrix.android.sdk.api.session.pushrules.RuleKind
import javax.inject.Inject

// Referenced in progressive_settings_preferences_root.xml
@AndroidEntryPoint
class ProgressiveSettingsNotifications :
        ProgressiveSettingsBaseFragment(),
        BackgroundSyncModeChooserDialog.InteractionListener {

    @Inject lateinit var unifiedPushHelper: UnifiedPushHelper
    @Inject lateinit var pushersManager: PushersManager
    @Inject lateinit var fcmHelper: FcmHelper
    @Inject lateinit var activeSessionHolder: ActiveSessionHolder
    @Inject lateinit var progressivePreferences: ProgressiveBasePreferences
    @Inject lateinit var guardServiceStarter: GuardServiceStarter
    @Inject lateinit var vectorFeatures: ProgressiveFeatures
    @Inject lateinit var notificationPermissionManager: NotificationPermissionManager
    @Inject lateinit var ensureFcmTokenIsRetrievedUseCase: EnsureFcmTokenIsRetrievedUseCase

    override var titleRes: Int = CommonStrings.settings_notifications
    override val preferenceXmlRes = R.xml.progressive_settings_notifications

    private var interactionListener: ProgressiveSettingsInteraction? = null

    private val viewModel: ProgressiveSettingsNotifViewModel by fragmentViewModel()

    private val notificationStartForActivityResult = registerStartForActivityResult { _ ->
        // No op
    }

    private val postPermissionLauncher = registerForPermissionsResult { _, deniedPermanently ->
        if (deniedPermanently) {
            // Open System setting, to give a chance to the user to enable notification. Sometimes the permission dialog is not displayed
            startNotificationSettingsIntent(requireContext(), notificationStartForActivityResult)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.SettingsNotifications
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        observeViewEvents()
    }

    private fun observeViewEvents() {
        viewModel.observeViewEvents {
            when (it) {
                ProgressiveSettingsNotifEvent.NotificationsForDeviceEnabled -> onNotificationsForDeviceEnabled()
                ProgressiveSettingsNotifEvent.NotificationsForDeviceDisabled -> onNotificationsForDeviceDisabled()
                is ProgressiveSettingsNotifEvent.AskUserForPushDistributor -> askUserToSelectPushDistributor()
                ProgressiveSettingsNotifEvent.NotificationMethodChanged -> onNotificationMethodChanged()
            }
        }
    }

    override fun bindPref() {
        findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_ENABLE_ALL_NOTIF_PREFERENCE_KEY)!!.let { pref ->
            val pushRuleService = session.pushRuleService()
            val mRuleMaster = pushRuleService.getPushRules().getAllRules()
                    .find { it.ruleId == RuleIds.RULE_ID_DISABLE_ALL }

            if (mRuleMaster == null) {
                // The homeserver does not support RULE_ID_DISABLE_ALL, so hide the preference
                pref.isVisible = false
                return
            }

            val areNotifEnabledAtAccountLevel = !mRuleMaster.enabled
            (pref as SwitchPreference).isChecked = areNotifEnabledAtAccountLevel
        }

        findPreference<SwitchPreference>(ProgressiveBasePreferences.SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY)
                ?.setOnPreferenceChangeListener { _, isChecked ->
                    val action = if (isChecked as Boolean) {
                        ProgressiveSettingsNotifAction.EnableNotificationsForDevice(pushDistributor = "")
                    } else {
                        ProgressiveSettingsNotifAction.DisableNotificationsForDevice
                    }
                    viewModel.handle(action)
                    // preference will be updated on ViewEvent reception
                    false
                }

        findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_FDROID_BACKGROUND_SYNC_MODE)?.let {
            it.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                val initialMode = progressivePreferences.getFdroidSyncBackgroundMode()
                val dialogFragment = BackgroundSyncModeChooserDialog.newInstance(initialMode)
                dialogFragment.interactionListener = this
                activity?.supportFragmentManager?.let { fm ->
                    dialogFragment.show(fm, "syncDialog")
                }
                true
            }
        }

        findPreference<ProgressiveEditTextPreference>(ProgressiveBasePreferences.SETTINGS_SET_SYNC_TIMEOUT_PREFERENCE_KEY)?.let {
            it.isEnabled = progressivePreferences.isBackgroundSyncEnabled()
            it.summary = secondsToText(progressivePreferences.backgroundSyncTimeOut())
            it.onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
                if (newValue is String) {
                    val syncTimeout = tryOrNull { Integer.parseInt(newValue) } ?: BackgroundSyncMode.DEFAULT_SYNC_TIMEOUT_SECONDS
                    progressivePreferences.setBackgroundSyncTimeout(maxOf(0, syncTimeout))
                    refreshBackgroundSyncPrefs()
                }
                true
            }
        }

        findPreference<ProgressiveEditTextPreference>(ProgressiveBasePreferences.SETTINGS_SET_SYNC_DELAY_PREFERENCE_KEY)?.let {
            it.isEnabled = progressivePreferences.isBackgroundSyncEnabled()
            it.summary = secondsToText(progressivePreferences.backgroundSyncDelay())
            it.onPreferenceChangeListener = Preference.OnPreferenceChangeListener { _, newValue ->
                if (newValue is String) {
                    val syncDelay = tryOrNull { Integer.parseInt(newValue) } ?: BackgroundSyncMode.DEFAULT_SYNC_DELAY_SECONDS
                    progressivePreferences.setBackgroundSyncDelay(maxOf(0, syncDelay))
                    refreshBackgroundSyncPrefs()
                }
                true
            }
        }

        findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_NOTIFICATION_METHOD_KEY)?.let {
            if (vectorFeatures.allowExternalUnifiedPushDistributors()) {
                it.summary = unifiedPushHelper.getCurrentDistributorName()
                it.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                    askUserToSelectPushDistributor(withUnregister = true)
                    true
                }
            } else {
                it.isVisible = false
            }
        }

        bindEmailNotifications()
        refreshBackgroundSyncPrefs()

        handleSystemPreference()
    }

    private fun onNotificationsForDeviceEnabled() {
        findPreference<SwitchPreference>(ProgressiveBasePreferences.SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY)
                ?.isChecked = true
        findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_NOTIFICATION_METHOD_KEY)
                ?.summary = unifiedPushHelper.getCurrentDistributorName()

        notificationPermissionManager.eventuallyRequestPermission(
                requireActivity(),
                postPermissionLauncher,
                showRationale = false,
                ignorePreference = true
        )
    }

    private fun onNotificationsForDeviceDisabled() {
        findPreference<SwitchPreference>(ProgressiveBasePreferences.SETTINGS_ENABLE_THIS_DEVICE_PREFERENCE_KEY)
                ?.isChecked = false
        notificationPermissionManager.eventuallyRevokePermission(requireActivity())
    }

    private fun askUserToSelectPushDistributor(withUnregister: Boolean = false) {
        unifiedPushHelper.showSelectDistributorDialog(requireContext()) { selection ->
            if (withUnregister) {
                viewModel.handle(ProgressiveSettingsNotifAction.RegisterPushDistributor(selection))
            } else {
                viewModel.handle(ProgressiveSettingsNotifAction.EnableNotificationsForDevice(selection))
            }
        }
    }

    private fun onNotificationMethodChanged() {
        findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_NOTIFICATION_METHOD_KEY)?.summary = unifiedPushHelper.getCurrentDistributorName()
        session.pushersService().refreshPushers()
        refreshBackgroundSyncPrefs()
    }

    private fun bindEmailNotifications() {
        val initialEmails = session.getEmailsWithPushInformation()
        bindEmailNotificationCategory(initialEmails)
        session.getEmailsWithPushInformationLive().observe(this) { emails ->
            if (initialEmails != emails) {
                bindEmailNotificationCategory(emails)
            }
        }
    }

    private fun bindEmailNotificationCategory(emails: List<Pair<ThreePid.Email, Boolean>>) {
        findPreference<ProgressivePreferenceCategory>(ProgressiveBasePreferences.SETTINGS_EMAIL_NOTIFICATION_CATEGORY_PREFERENCE_KEY)?.let { category ->
            category.removeAll()
            if (emails.isEmpty()) {
                val vectorPreference = ProgressiveBasePreference(requireContext())
                vectorPreference.title = resources.getString(CommonStrings.settings_notification_emails_no_emails)
                category.addPreference(vectorPreference)
                vectorPreference.setOnPreferenceClickListener {
                    interactionListener?.navigateToEmailAndPhoneNumbers()
                    true
                }
            } else {
                emails.forEach { (emailPid, isEnabled) ->
                    val pref = ProgressiveSwitchPreference(requireContext())
                    pref.title = resources.getString(CommonStrings.settings_notification_emails_enable_for_email, emailPid.email)
                    pref.isChecked = isEnabled
                    pref.setTransactionalSwitchChangeListener(lifecycleScope) { isChecked ->
                        if (isChecked) {
                            pushersManager.registerEmailForPush(emailPid.email)
                        } else {
                            pushersManager.unregisterEmailPusher(emailPid.email)
                        }
                    }
                    category.addPreference(pref)
                }
            }
        }
    }

    private val batteryStartForActivityResult = registerStartForActivityResult {
        // Noop
    }

    // BackgroundSyncModeChooserDialog.InteractionListener
    override fun onOptionSelected(mode: BackgroundSyncMode) {
        // option has change, need to act
        if (mode == BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_REALTIME) {
            // Important, Battery optim white listing is needed in this mode;
            // Even if using foreground service with foreground notif, it stops to work
            // in doze mode for certain devices :/
            if (!requireContext().isIgnoringBatteryOptimizations()) {
                requestDisablingBatteryOptimization(requireActivity(), batteryStartForActivityResult)
            }
        }
        progressivePreferences.setFdroidSyncBackgroundMode(mode)
        refreshBackgroundSyncPrefs()
    }

    private fun refreshBackgroundSyncPrefs() {
        findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_FDROID_BACKGROUND_SYNC_MODE)?.let {
            it.summary = when (progressivePreferences.getFdroidSyncBackgroundMode()) {
                BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_BATTERY -> getString(CommonStrings.settings_background_fdroid_sync_mode_battery)
                BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_FOR_REALTIME -> getString(CommonStrings.settings_background_fdroid_sync_mode_real_time)
                BackgroundSyncMode.FDROID_BACKGROUND_SYNC_MODE_DISABLED -> getString(CommonStrings.settings_background_fdroid_sync_mode_disabled)
            }
        }

        findPreference<ProgressivePreferenceCategory>(ProgressiveBasePreferences.SETTINGS_BACKGROUND_SYNC_PREFERENCE_KEY)?.let {
            it.isVisible = unifiedPushHelper.isBackgroundSync()
        }

        val backgroundSyncEnabled = progressivePreferences.isBackgroundSyncEnabled()
        findPreference<ProgressiveEditTextPreference>(ProgressiveBasePreferences.SETTINGS_SET_SYNC_TIMEOUT_PREFERENCE_KEY)?.let {
            it.isEnabled = backgroundSyncEnabled
            it.summary = secondsToText(progressivePreferences.backgroundSyncTimeOut())
        }
        findPreference<ProgressiveEditTextPreference>(ProgressiveBasePreferences.SETTINGS_SET_SYNC_DELAY_PREFERENCE_KEY)?.let {
            it.isEnabled = backgroundSyncEnabled
            it.summary = secondsToText(progressivePreferences.backgroundSyncDelay())
        }
        when {
            backgroundSyncEnabled -> guardServiceStarter.start()
            else -> guardServiceStarter.stop()
        }
    }

    /**
     * Convert a delay in seconds to string.
     *
     * @param seconds the delay in seconds
     * @return the text
     */
    private fun secondsToText(seconds: Int): String {
        return resources.getQuantityString(CommonPlurals.seconds, seconds, seconds)
    }

    private fun handleSystemPreference() {
        val callNotificationsSystemOptions = findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_SYSTEM_CALL_NOTIFICATION_PREFERENCE_KEY)!!
        if (NotificationUtils.supportNotificationChannels()) {
            callNotificationsSystemOptions.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                NotificationUtils.openSystemSettingsForCallCategory(this)
                false
            }
        } else {
            callNotificationsSystemOptions.isVisible = false
        }

        val noisyNotificationsSystemOptions = findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_SYSTEM_NOISY_NOTIFICATION_PREFERENCE_KEY)!!
        if (NotificationUtils.supportNotificationChannels()) {
            noisyNotificationsSystemOptions.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                NotificationUtils.openSystemSettingsForNoisyCategory(this)
                false
            }
        } else {
            noisyNotificationsSystemOptions.isVisible = false
        }

        val silentNotificationsSystemOptions = findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_SYSTEM_SILENT_NOTIFICATION_PREFERENCE_KEY)!!
        if (NotificationUtils.supportNotificationChannels()) {
            silentNotificationsSystemOptions.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                NotificationUtils.openSystemSettingsForSilentCategory(this)
                false
            }
        } else {
            silentNotificationsSystemOptions.isVisible = false
        }

        // Ringtone
        val ringtonePreference = findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_NOTIFICATION_RINGTONE_SELECTION_PREFERENCE_KEY)!!

        if (NotificationUtils.supportNotificationChannels()) {
            ringtonePreference.isVisible = false
        } else {
            ringtonePreference.summary = progressivePreferences.getNotificationRingToneName()
            ringtonePreference.onPreferenceClickListener = Preference.OnPreferenceClickListener {
                val intent = Intent(RingtoneManager.ACTION_RINGTONE_PICKER)
                intent.putExtra(RingtoneManager.EXTRA_RINGTONE_TYPE, RingtoneManager.TYPE_NOTIFICATION)

                if (null != progressivePreferences.getNotificationRingTone()) {
                    intent.putExtra(RingtoneManager.EXTRA_RINGTONE_EXISTING_URI, progressivePreferences.getNotificationRingTone())
                }

                ringtoneStartForActivityResult.launch(intent)
                false
            }
        }
    }

    private val ringtoneStartForActivityResult = registerStartForActivityResult { activityResult ->
        if (activityResult.resultCode == Activity.RESULT_OK) {
            progressivePreferences.setNotificationRingTone(activityResult.data?.getParcelableExtraCompat<Uri>(RingtoneManager.EXTRA_RINGTONE_PICKED_URI))

            // test if the selected ring tone can be played
            val notificationRingToneName = progressivePreferences.getNotificationRingToneName()
            if (null != notificationRingToneName) {
                progressivePreferences.setNotificationRingTone(progressivePreferences.getNotificationRingTone())
                findPreference<ProgressiveBasePreference>(ProgressiveBasePreferences.SETTINGS_NOTIFICATION_RINGTONE_SELECTION_PREFERENCE_KEY)!!
                        .summary = notificationRingToneName
            }
        }
    }

    override fun onResume() {
        super.onResume()
        activeSessionHolder.getSafeActiveSession()?.pushersService()?.refreshPushers()

        interactionListener?.requestedKeyToHighlight()?.let { key ->
            interactionListener?.requestHighlightPreferenceKeyOnResume(null)
            val preference = findPreference<ProgressiveSwitchPreference>(key)
            preference?.isHighlighted = true
        }

        refreshPref()
    }

    private fun refreshPref() {
        // This pref may have change from troubleshoot pref fragment
        if (unifiedPushHelper.isBackgroundSync()) {
            findPreference<ProgressiveSwitchPreference>(ProgressiveBasePreferences.SETTINGS_START_ON_BOOT_PREFERENCE_KEY)
                    ?.isChecked = progressivePreferences.autoStartOnBoot()
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is ProgressiveSettingsInteraction) {
            interactionListener = context
        }
        (activity?.supportFragmentManager
                ?.findFragmentByTag("syncDialog") as BackgroundSyncModeChooserDialog?)
                ?.interactionListener = this
    }

    override fun onDetach() {
        interactionListener = null
        super.onDetach()
    }

    override fun onPreferenceTreeClick(preference: Preference): Boolean {
        return when (preference.key) {
            ProgressiveBasePreferences.SETTINGS_ENABLE_ALL_NOTIF_PREFERENCE_KEY -> {
                updateEnabledForAccount(preference)
                true
            }
            else -> {
                return super.onPreferenceTreeClick(preference)
            }
        }
    }

    private fun updateEnabledForAccount(preference: Preference?) {
        val pushRuleService = session.pushRuleService()
        val switchPref = preference as SwitchPreference
        pushRuleService.getPushRules().getAllRules()
                .find { it.ruleId == RuleIds.RULE_ID_DISABLE_ALL }
                ?.let {
                    // Trick, we must enable this room to disable notifications
                    lifecycleScope.launch {
                        try {
                            pushRuleService.updatePushRuleEnableStatus(
                                    RuleKind.OVERRIDE,
                                    it,
                                    !switchPref.isChecked
                            )
                            // Push rules will be updated from the sync
                        } catch (failure: Throwable) {
                            if (!isAdded) {
                                return@launch
                            }

                            // revert the check box
                            switchPref.isChecked = !switchPref.isChecked
                            Toast.makeText(activity, CommonStrings.unknown_error, Toast.LENGTH_SHORT).show()
                        }
                    }
                }
    }
}

private fun SwitchPreference.setTransactionalSwitchChangeListener(scope: CoroutineScope, transaction: suspend (Boolean) -> Unit) {
    setOnPreferenceChangeListener { switchPreference, isChecked ->
        require(switchPreference is SwitchPreference)
        val originalState = switchPreference.isChecked
        scope.launch {
            try {
                transaction(isChecked as Boolean)
            } catch (failure: Throwable) {
                switchPreference.isChecked = originalState
                Toast.makeText(switchPreference.context, CommonStrings.unknown_error, Toast.LENGTH_SHORT).show()
            }
        }
        true
    }
}

/**
 * Fetches the current users 3pid emails and pairs them with their enabled state.
 * If no pusher is available for a given email we can infer that push is not registered for the email.
 * @return a list of ThreePid emails paired with the email notification enabled state. true if email notifications are enabled, false if not.
 * @see ThreePid.Email
 */
private fun Session.getEmailsWithPushInformation(): List<Pair<ThreePid.Email, Boolean>> {
    val emailPushers = pushersService().getPushers().filter { it.kind == Pusher.KIND_EMAIL }
    return profileService().getThreePids()
            .filterIsInstance<ThreePid.Email>()
            .map { it to emailPushers.any { pusher -> pusher.pushKey == it.email } }
}

private fun Session.getEmailsWithPushInformationLive(): LiveData<List<Pair<ThreePid.Email, Boolean>>> {
    val emailThreePids = profileService().getThreePidsLive(refreshData = true).map { it.filterIsInstance<ThreePid.Email>() }
    val emailPushers = pushersService().getPushersLive().map { it.filter { pusher -> pusher.kind == Pusher.KIND_EMAIL } }
    return combineLatest(emailThreePids, emailPushers) { emailThreePidsResult, emailPushersResult ->
        emailThreePidsResult.map { it to emailPushersResult.any { pusher -> pusher.pushKey == it.email } }
    }.distinctUntilChanged()
}
