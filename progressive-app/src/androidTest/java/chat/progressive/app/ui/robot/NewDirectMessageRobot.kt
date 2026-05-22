/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.ui.robot

import androidx.test.espresso.Espresso
import androidx.test.espresso.matcher.ViewMatchers
import chat.progressive.app.R
import chat.progressive.app.waitForView
import chat.progressive.lib.strings.CommonStrings

class NewDirectMessageRobot {

    fun verifyQrCodeButton() {
        Espresso.onView(ViewMatchers.withId(R.id.userListRecyclerView))
                .perform(waitForView(ViewMatchers.withText(CommonStrings.qr_code)))
    }

    fun verifyInviteFriendsButton() {
        Espresso.onView(ViewMatchers.withId(R.id.userListRecyclerView))
                .perform(waitForView(ViewMatchers.withText(CommonStrings.invite_friends)))
    }
}
