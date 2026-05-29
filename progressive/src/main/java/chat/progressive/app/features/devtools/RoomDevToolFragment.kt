/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.devtools

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.airbnb.mvrx.activityViewModel
import dagger.hilt.android.AndroidEntryPoint
import chat.progressive.app.R
import chat.progressive.app.core.extensions.cleanup
import chat.progressive.app.core.extensions.configureWith
import chat.progressive.app.core.platform.ProgressiveFragment
import chat.progressive.app.databinding.FragmentGenericRecyclerBinding
import javax.inject.Inject

@AndroidEntryPoint
class RoomDevToolFragment :
        ProgressiveFragment<FragmentGenericRecyclerBinding>(),
        DevToolsInteractionListener {

    @Inject lateinit var epoxyController: RoomDevToolRootController

    private val sharedViewModel: RoomDevToolViewModel by activityViewModel()

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentGenericRecyclerBinding {
        return FragmentGenericRecyclerBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        views.genericRecyclerView.configureWith(epoxyController, dividerDrawable = R.drawable.divider_horizontal)
        epoxyController.interactionListener = this

//        sharedViewModel.observeViewEvents {
//            when (it) {
//                is DevToolsViewEvents.showJson -> {
//                    JSonViewerDialog.newInstance(it.jsonString, -1, createJSonViewerStyleProvider(colorProvider))
//                            .show(childFragmentManager, "JSON_VIEWER")
//                }
//            }
//        }
    }

    override fun onDestroyView() {
        views.genericRecyclerView.cleanup()
        epoxyController.interactionListener = null
        super.onDestroyView()
    }

    override fun processAction(action: RoomDevToolAction) {
        sharedViewModel.handle(action)
    }
}
