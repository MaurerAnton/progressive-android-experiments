/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.debug.analytics

import android.view.View
import dagger.hilt.android.AndroidEntryPoint
import chat.progressive.app.core.extensions.addFragment
import chat.progressive.app.core.platform.VectorBaseActivity
import chat.progressive.app.databinding.ActivitySimpleBinding

@AndroidEntryPoint
class DebugAnalyticsActivity : VectorBaseActivity<ActivitySimpleBinding>() {

    override fun getBinding() = ActivitySimpleBinding.inflate(layoutInflater)

    override fun getCoordinatorLayout() = views.coordinatorLayout
    override val rootView: View
        get() = views.coordinatorLayout

    override fun initUiAndData() {
        if (isFirstCreation()) {
            addFragment(
                    views.simpleFragmentContainer,
                    DebugAnalyticsFragment::class.java
            )
        }
    }
}
