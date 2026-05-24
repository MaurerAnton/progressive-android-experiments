/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.attachments

import com.airbnb.mvrx.test.MavericksTestRule
import chat.progressive.app.test.fakes.FakeProgressiveFeatures
import chat.progressive.app.test.fakes.FakeProgressivePreferences
import chat.progressive.app.test.test
import io.mockk.verifyOrder
import org.junit.Before
import org.junit.Rule
import org.junit.Test

internal class AttachmentTypeSelectorViewModelTest {

    @get:Rule
    val mavericksTestRule = MavericksTestRule()

    private val fakeProgressiveFeatures = FakeProgressiveFeatures()
    private val fakeProgressivePreferences = FakeProgressivePreferences()
    private val initialState = AttachmentTypeSelectorViewState()

    @Before
    fun setUp() {
        // Disable all features by default
        fakeProgressiveFeatures.givenLocationSharing(isEnabled = false)
        fakeProgressiveFeatures.givenVoiceBroadcast(isEnabled = false)
        fakeProgressivePreferences.givenTextFormatting(isEnabled = false)
    }

    @Test
    fun `given features are not enabled, then options are not visible`() {
        createViewModel()
                .test()
                .assertStates(
                        listOf(
                                initialState,
                        )
                )
                .finish()
    }

    @Test
    fun `given location sharing is enabled, then location sharing option is visible`() {
        fakeProgressiveFeatures.givenLocationSharing(isEnabled = true)

        createViewModel()
                .test()
                .assertStates(
                        listOf(
                                initialState.copy(
                                        isLocationVisible = true
                                ),
                        )
                )
                .finish()
    }

    @Test
    fun `given voice broadcast is enabled, then voice broadcast option is visible`() {
        fakeProgressiveFeatures.givenVoiceBroadcast(isEnabled = true)

        createViewModel()
                .test()
                .assertStates(
                        listOf(
                                initialState.copy(
                                        isVoiceBroadcastVisible = true
                                ),
                        )
                )
                .finish()
    }

    @Test
    fun `given text formatting is enabled, then text formatting option is checked`() {
        fakeProgressivePreferences.givenTextFormatting(isEnabled = true)

        createViewModel()
                .test()
                .assertStates(
                        listOf(
                                initialState.copy(
                                        isTextFormattingEnabled = true
                                ),
                        )
                )
                .finish()
    }

    @Test
    fun `when text formatting is changed, then it updates the UI`() {
        createViewModel()
                .apply {
                    handle(AttachmentTypeSelectorAction.ToggleTextFormatting(isEnabled = true))
                }
                .test()
                .assertStates(
                        listOf(
                                initialState.copy(
                                        isTextFormattingEnabled = true
                                ),
                        )
                )
                .finish()
    }

    @Test
    fun `when text formatting is changed, then it persists the change`() {
        createViewModel()
                .apply {
                    handle(AttachmentTypeSelectorAction.ToggleTextFormatting(isEnabled = true))
                    handle(AttachmentTypeSelectorAction.ToggleTextFormatting(isEnabled = false))
                }
        verifyOrder {
            fakeProgressivePreferences.instance.setTextFormattingEnabled(true)
            fakeProgressivePreferences.instance.setTextFormattingEnabled(false)
        }
    }

    private fun createViewModel(): AttachmentTypeSelectorViewModel {
        return AttachmentTypeSelectorViewModel(
                initialState,
                vectorFeatures = fakeProgressiveFeatures,
                vectorPreferences = fakeProgressivePreferences.instance,
        )
    }
}
