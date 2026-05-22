/*
 * Copyright 2022-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.test.fakes

import chat.progressive.app.features.DefaultVectorOverrides
import chat.progressive.app.features.VectorOverrides

class FakeProgressiveOverrides : VectorOverrides by DefaultVectorOverrides()
