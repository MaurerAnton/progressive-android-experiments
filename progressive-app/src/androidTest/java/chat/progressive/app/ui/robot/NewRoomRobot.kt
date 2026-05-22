/*
 * Copyright 2021-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.ui.robot

import androidx.test.espresso.Espresso.pressBack
import androidx.test.espresso.matcher.ViewMatchers.withId
import com.adevinta.android.barista.interaction.BaristaClickInteractions.clickOn
import chat.progressive.app.R
import chat.progressive.app.espresso.tools.waitUntilViewVisible
import chat.progressive.app.features.DefaultVectorFeatures
import chat.progressive.app.features.VectorFeatures
import chat.progressive.app.ui.robot.settings.labs.LabFeaturesPreferences
import chat.progressive.lib.strings.CommonStrings

class NewRoomRobot(
        var createdRoom: Boolean = false,
        private val labsPreferences: LabFeaturesPreferences
) {
    private val features: VectorFeatures = DefaultVectorFeatures()

    fun createNewRoom(block: CreateNewRoomRobot.() -> Unit) {
        clickOn(CommonStrings.create_new_room)
        waitUntilViewVisible(withId(R.id.createRoomForm))
        val createNewRoomRobot = CreateNewRoomRobot()
        block(createNewRoomRobot)
        createdRoom = createNewRoomRobot.createdRoom
        if (!createNewRoomRobot.createdRoom) {
            pressBack()
        }
    }
}
