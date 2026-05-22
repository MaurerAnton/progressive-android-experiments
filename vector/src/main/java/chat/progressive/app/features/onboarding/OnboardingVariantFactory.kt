/*
 * Copyright 2021-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.features.onboarding

import chat.progressive.app.config.OnboardingVariant
import chat.progressive.app.core.platform.ScreenOrientationLocker
import chat.progressive.app.core.resources.BuildMeta
import chat.progressive.app.databinding.ActivityLoginBinding
import chat.progressive.app.features.ProgressiveFeatures
import chat.progressive.app.features.onboarding.ftueauth.FtueAuthVariant
import javax.inject.Inject

class OnboardingVariantFactory @Inject constructor(
        private val vectorFeatures: ProgressiveFeatures,
        private val orientationLocker: ScreenOrientationLocker,
        private val buildMeta: BuildMeta,
) {

    fun create(
            activity: OnboardingActivity,
            views: ActivityLoginBinding,
            onboardingViewModel: Lazy<OnboardingViewModel>,
    ) = when (vectorFeatures.onboardingVariant()) {
        OnboardingVariant.LEGACY -> error("Legacy is not supported by the FTUE")
        OnboardingVariant.FTUE_AUTH -> FtueAuthVariant(
                views = views,
                onboardingViewModel = onboardingViewModel.value,
                activity = activity,
                supportFragmentManager = activity.supportFragmentManager,
                vectorFeatures = vectorFeatures,
                orientationLocker = orientationLocker,
                buildMeta = buildMeta,
        )
    }
}
