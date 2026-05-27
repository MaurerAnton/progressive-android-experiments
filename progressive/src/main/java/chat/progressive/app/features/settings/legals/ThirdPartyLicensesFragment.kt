/*
 * Copyright 2024-2026 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package chat.progressive.app.features.settings.legals

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import dagger.hilt.android.AndroidEntryPoint
import chat.progressive.app.core.extensions.cleanup
import chat.progressive.app.core.extensions.configureWith
import chat.progressive.app.core.platform.ProgressiveFragment
import chat.progressive.app.databinding.FragmentGenericRecyclerBinding
import chat.progressive.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * Displays third-party open source licenses natively in a RecyclerView
 * instead of loading open_source_licenses.html in a WebView.
 */
@AndroidEntryPoint
class ThirdPartyLicensesFragment :
    ProgressiveFragment<FragmentGenericRecyclerBinding>() {

    @Inject lateinit var controller: ThirdPartyLicensesController

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentGenericRecyclerBinding {
        return FragmentGenericRecyclerBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        (activity as? AppCompatActivity)?.supportActionBar?.setTitle(CommonStrings.settings_third_party_notices)

        val licenses = loadLicenses()
        controller.setData(licenses)
        views.genericRecyclerView.configureWith(controller)
    }

    override fun onDestroyView() {
        views.genericRecyclerView.cleanup()
        super.onDestroyView()
    }

    private fun loadLicenses(): List<LicenseEntry> {
        val html = requireContext().assets.open("open_source_licenses.html")
            .bufferedReader().readText()
        return LicenseParser.parse(html)
    }
}
