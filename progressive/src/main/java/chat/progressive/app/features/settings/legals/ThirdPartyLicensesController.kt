/*
 * Copyright 2024-2026 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */
package chat.progressive.app.features.settings.legals

import com.airbnb.epoxy.TypedEpoxyController
import chat.progressive.app.features.discovery.settingsSectionTitleItem
import chat.progressive.lib.strings.CommonStrings
import javax.inject.Inject

class ThirdPartyLicensesController @Inject constructor() :
    TypedEpoxyController<List<LicenseEntry>>() {

    override fun buildModels(licenses: List<LicenseEntry>) {
        settingsSectionTitleItem {
            id("title")
            titleResId(CommonStrings.settings_third_party_notices)
        }

        licenses.forEachIndexed { index, license ->
            licenseItem {
                id("license_$index")
                name(license.name)
                copyright(license.copyright)
                licenseText(license.licenseText)
            }
        }
    }
}
