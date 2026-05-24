/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.settings.devices.v2

import chat.progressive.app.test.fakes.FakeProgressivePreferences
import org.junit.Test

class ToggleIpAddressVisibilityUseCaseTest {

    private val fakeProgressivePreferences = FakeProgressivePreferences()

    private val toggleIpAddressVisibilityUseCase = ToggleIpAddressVisibilityUseCase(
            vectorPreferences = fakeProgressivePreferences.instance,
    )

    @Test
    fun `given ip addresses are currently visible then then visibility is set as false`() {
        // Given
        fakeProgressivePreferences.givenShowIpAddressInSessionManagerScreens(true)

        // When
        toggleIpAddressVisibilityUseCase.execute()

        // Then
        fakeProgressivePreferences.verifySetIpAddressVisibilityInDeviceManagerScreens(false)
    }

    @Test
    fun `given ip addresses are currently not visible then then visibility is set as true`() {
        // Given
        fakeProgressivePreferences.givenShowIpAddressInSessionManagerScreens(false)

        // When
        toggleIpAddressVisibilityUseCase.execute()

        // Then
        fakeProgressivePreferences.verifySetIpAddressVisibilityInDeviceManagerScreens(true)
    }
}
