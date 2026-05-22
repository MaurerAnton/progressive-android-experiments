/*
 * Copyright 2020-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.account.deactivation

import android.app.Activity
import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import com.airbnb.mvrx.fragmentViewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.progressive.app.core.extensions.registerStartForActivityResult
import chat.progressive.app.core.platform.ProgressiveFragment
import chat.progressive.app.databinding.FragmentDeactivateAccountBinding
import chat.progressive.app.features.MainActivity
import chat.progressive.app.features.MainActivityArgs
import chat.progressive.app.features.analytics.plan.MobileScreen
import chat.progressive.app.features.auth.ReAuthActivity
import chat.progressive.app.features.settings.ProgressiveSettingsActivity
import chat.progressive.lib.strings.CommonStrings
import org.matrix.android.sdk.api.auth.data.LoginFlowTypes
import org.matrix.android.sdk.api.session.uia.exceptions.UiaCancelledException

@AndroidEntryPoint
class DeactivateAccountFragment :
        ProgressiveFragment<FragmentDeactivateAccountBinding>() {

    private val viewModel: DeactivateAccountViewModel by fragmentViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentDeactivateAccountBinding {
        return FragmentDeactivateAccountBinding.inflate(inflater, container, false)
    }

    private val reAuthActivityResultLauncher = registerStartForActivityResult { activityResult ->
        if (activityResult.resultCode == Activity.RESULT_OK) {
            when (activityResult.data?.extras?.getString(ReAuthActivity.RESULT_FLOW_TYPE)) {
                LoginFlowTypes.SSO -> {
                    viewModel.handle(DeactivateAccountAction.SsoAuthDone)
                }
                LoginFlowTypes.PASSWORD -> {
                    val password = activityResult.data?.extras?.getString(ReAuthActivity.RESULT_VALUE) ?: ""
                    viewModel.handle(DeactivateAccountAction.PasswordAuthDone(password))
                }
                else -> {
                    viewModel.handle(DeactivateAccountAction.ReAuthCancelled)
                }
            }
        } else {
            viewModel.handle(DeactivateAccountAction.ReAuthCancelled)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        analyticsScreenName = MobileScreen.ScreenName.DeactivateAccount
    }

    override fun onResume() {
        super.onResume()
        (activity as? AppCompatActivity)?.supportActionBar?.setTitle(CommonStrings.deactivate_account_title)
    }

    private var settingsActivity: ProgressiveSettingsActivity? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        settingsActivity = context as? ProgressiveSettingsActivity
    }

    override fun onDetach() {
        super.onDetach()
        settingsActivity = null
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        setupViewListeners()
        observeViewEvents()
    }

    private fun setupViewListeners() {
        views.deactivateAccountSubmit.debouncedClicks {
            viewModel.handle(
                    DeactivateAccountAction.DeactivateAccount(
                            views.deactivateAccountEraseCheckbox.isChecked
                    )
            )
        }
    }

    private fun observeViewEvents() {
        viewModel.observeViewEvents {
            when (it) {
                is DeactivateAccountViewEvents.Loading -> {
                    settingsActivity?.ignoreInvalidTokenError = true
                    showLoadingDialog(it.message)
                }
                DeactivateAccountViewEvents.InvalidAuth -> {
                    dismissLoadingDialog()
                    settingsActivity?.ignoreInvalidTokenError = false
                }
                is DeactivateAccountViewEvents.OtherFailure -> {
                    settingsActivity?.ignoreInvalidTokenError = false
                    dismissLoadingDialog()
                    if (it.throwable !is UiaCancelledException) {
                        displayErrorDialog(it.throwable)
                    }
                }
                DeactivateAccountViewEvents.Done -> {
                    MainActivity.restartApp(requireActivity(), MainActivityArgs(clearCredentials = true, isAccountDeactivated = true))
                }
                is DeactivateAccountViewEvents.RequestReAuth -> {
                    ReAuthActivity.newIntent(
                            requireContext(),
                            it.registrationFlowResponse,
                            it.lastErrorCode,
                            getString(CommonStrings.deactivate_account_title)
                    ).let { intent ->
                        reAuthActivityResultLauncher.launch(intent)
                    }
                }
            }
        }
    }
}
