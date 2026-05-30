package chat.progressive.app.native

import org.json.JSONObject
import org.json.JSONArray
import timber.log.Timber

/**
 * JNI bridge to the progressive_native C++ library.
 * Handles date validation, MSC3030 URL construction, and response parsing.
 */
object ProgressiveNative {

    private var isLoaded = false

    fun ensureLoaded() {
        if (!isLoaded) {
            try {
                System.loadLibrary("progressive_native")
                isLoaded = true
                Timber.d("progressive_native loaded successfully")
            } catch (e: UnsatisfiedLinkError) {
                Timber.e(e, "Failed to load progressive_native")
                // Fallback: use pure Kotlin implementation
            }
        }
    }

    /**
     * Validates the date string (YYYY-MM-DD) and builds the MSC3030 URL.
     * Gates on the feature flag from C++ side.
     *
     * @param roomId Target room ID
     * @param dateString Date in YYYY-MM-DD format
     * @param serverUrl Base URL of the homeserver
     * @param accessToken User's access token
     * @param isEnabled Whether jumptodate labs setting is enabled
     *
     * @return JSON string with url, accessToken, timestamp — or {"error":"..."} on failure
     */
    @JvmStatic
        roomId: String,
        dateString: String,
        serverUrl: String,
        accessToken: String,
        isEnabled: Boolean
    ): String

    /**
     * Parses the UTC timestamp_to_event response from the homeserver.
     *
     * @param responseBody HTTP response body
     * @param httpStatus HTTP status code
     *
     * @return JSON string with eventId — or error info on failure
     */
    @JvmStatic
        responseBody: String,
        httpStatus: Int
    ): String

    /**
     * Parses a Matrix event JSON to find the source event it relates to
     * (reply, reaction, edit, thread root).
     *
     * @param eventJson Full JSON of the Matrix event
     * @param allowedTypes Comma-separated list of allowed relation types, or empty for all
     *
     * @return JSON with sourceEventId and relationType, or {"isRelation": false}
     */
    @JvmStatic
    external fun nativeParseRelation(
        eventJson: String,
        allowedTypes: String
    ): String

    // --- Export functions ---

    @JvmStatic
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        fileName: String,
        mediaSize: String,
        relationType: String,
        isContinuation: Boolean
    ): String

    @JvmStatic
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        fileName: String,
        relationType: String
    ): String

    @JvmStatic
        roomName: String,
        roomTopic: String,
        exportDate: String,
        eventHtmls: Array<String>
    ): String

    // --- Event Cache ---

    @JvmStatic
        eventId: String,
        senderId: String,
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        eventType: String,
        relationType: String,
        sourceEventId: String,
        sentByMe: Boolean
    )

    @JvmStatic

    @JvmStatic

    @JvmStatic

    // --- SQLite Event Database ---

    @JvmStatic
    external fun nativeDbOpen(dbPath: String): Boolean

    @JvmStatic
    external fun nativeDbClose()

    @JvmStatic
    external fun nativeDbInsertEvent(
        eventId: String, roomId: String, senderId: String, senderName: String,
        timestamp: String, body: String, msgType: String, eventType: String,
        relationType: String, sourceEventId: String,
        originServerTs: Long, displayIndex: Int, sentByMe: Boolean
    )

    @JvmStatic
    external fun nativeDbGetContext(eventId: String): String

    @JvmStatic
    external fun nativeDbClearRoom(roomId: String)

    @JvmStatic
    external fun nativeDbCount(): Int

    // --- Native SQLite DB (SqliteDB - richer API with room summaries, transactions) ---

    @JvmStatic external fun nativeSqliteDbOpen(dbPath: String, key: String): Boolean
    @JvmStatic external fun nativeSqliteDbClose(key: String)
    @JvmStatic external fun nativeSqliteDbInsertEvent(
        key: String, eventId: String, roomId: String, type: String, senderId: String,
        contentJson: String, originTs: Long, ageTs: Long, displayIndex: Int
    ): Boolean
    @JvmStatic external fun nativeSqliteDbInsertEventRel(
        key: String, eventId: String, roomId: String, type: String, senderId: String,
        contentJson: String, originTs: Long, ageTs: Long, displayIndex: Int,
        stateKey: String, redacts: String, relType: String, relatesToId: String
    ): Boolean
    @JvmStatic external fun nativeSqliteDbQueryEvents(key: String, roomId: String, limit: Int, offset: Int, ascending: Boolean): String
    @JvmStatic external fun nativeSqliteDbQueryEvent(key: String, eventId: String): String
    @JvmStatic external fun nativeSqliteDbDeleteEvent(key: String, eventId: String)
    @JvmStatic external fun nativeSqliteDbCountEvents(key: String, roomId: String): Int
    @JvmStatic external fun nativeSqliteDbMaxDisplayIndex(key: String, roomId: String): Int
    @JvmStatic external fun nativeSqliteDbUpsertRoom(
        key: String, roomId: String, displayName: String, avatarUrl: String,
        topic: String, membership: String, notifCount: Int, highlightCount: Int,
        lastActivityMs: Long, isDirect: Boolean, isSpace: Boolean, isFavourite: Boolean, isEncrypted: Boolean
    ): Boolean
    @JvmStatic external fun nativeSqliteDbQueryRooms(key: String): String
    @JvmStatic external fun nativeSqliteDbBeginTransaction(key: String)
    @JvmStatic external fun nativeSqliteDbCommitTransaction(key: String)
    @JvmStatic external fun nativeSqliteDbSchemaVersion(key: String): Int

    // --- Translation ---

    @JvmStatic
    external fun nativeBuildTranslateRequest(
        text: String,
        sourceLang: String,
        targetLang: String,
        apiEndpoint: String,
        apiToken: String,
        model: String
    ): String

    @JvmStatic
    external fun nativeParseTranslateResponse(
        responseBody: String,
        httpStatus: Int
    ): String

    // --- Proxy / Tor / I2P ---

    @JvmStatic
        connType: Int,
        proxyType: Int,
        host: String,
        port: Int,
        username: String,
        password: String
    ): String

    // --- Yggdrasil ---


    // --- Markdown ---

    @JvmStatic external fun nativeMarkdownToHtml(markdown: String, enableTables: Boolean): String
    @JvmStatic external fun nativeParseMarkdownTable(tableBlock: String, withScroll: Boolean): String

    // --- Event Relations ---

    @JvmStatic external fun nativeIsReply(contentJson: String): Boolean
    @JvmStatic external fun nativeIsEdit(contentJson: String): Boolean
    @JvmStatic external fun nativeIsReaction(contentJson: String): Boolean
    @JvmStatic external fun nativeIsThreadRoot(contentJson: String): Boolean
    @JvmStatic external fun nativeExtractThreadRoot(contentJson: String): String
    @JvmStatic external fun nativeExtractReplySource(contentJson: String): String
    @JvmStatic external fun nativeExtractEditSource(contentJson: String): String
    @JvmStatic external fun nativeBuildReplyRelationWithThread(eventId: String, threadRoot: String): String

    // --- Room Content Parsers ---

    @JvmStatic external fun nativeParseRoomNameContent(contentJson: String): String
    @JvmStatic external fun nativeParseRoomTopicContent(contentJson: String): String
    @JvmStatic external fun nativeParseRoomAvatarContent(contentJson: String): String

    // --- Connection Monitor ---

    @JvmStatic external fun nativeFormatDowntime(downtimeMs: Long): String
    @JvmStatic external fun nativeGetBannerColor(downtimeMs: Long): String

    // --- Content Guard ---

    @JvmStatic external fun nativeCountEmojis(text: String): Int
    @JvmStatic external fun nativeCountUniqueEmojis(text: String): Int
    @JvmStatic external fun nativeFormatMediaCollapseLabel(count: Int): String
    @JvmStatic external fun nativeIsEmojiCodePoint(codepoint: Int): Boolean

    // --- MIME Normalization ---

    @JvmStatic external fun nativeNormalizeMimeType(mimeType: String): String

    // --- Room State Parse ---

    @JvmStatic external fun nativeParseJoinRules(contentJson: String): String
    @JvmStatic external fun nativeParseHistoryVisibility(contentJson: String): String
    @JvmStatic external fun nativeParseGuestAccess(contentJson: String): String

    // --- Push Rules ---

    @JvmStatic external fun nativeIsKnownPushRuleKind(kind: String): Boolean
    @JvmStatic external fun nativeGetRuleKindDescription(kind: String, enabled: Boolean): String
    @JvmStatic external fun nativeIsMsc3061SharedKey(roomKeyContentJson: String): Boolean
    @JvmStatic external fun nativeFormatMsc3061Status(isShared: Boolean, visibilitySetting: String): String
    @JvmStatic external fun nativeCanShareHistory(roomVisibility: String): Boolean

    // --- Poll Utilities ---

    @JvmStatic external fun nativeGeneratePollOptionId(): String

    // --- Identity Utilities ---

    @JvmStatic external fun nativeDisambiguateName(displayName: String, mxid: String): String
    @JvmStatic external fun nativeGetIdentityInitials(displayName: String): String
    @JvmStatic external fun nativeIsCanonicalAlias(alias: String, expectedRoomId: String): Boolean
    @JvmStatic external fun nativeSuggestAliases(roomName: String): String

    // --- Presence / Backup / Cross-Signing (JSON round-trip) ---

    @JvmStatic external fun nativeParsePresenceInfo(userId: String, apiResponseJson: String): String
    @JvmStatic external fun nativeParseBackupInfo(apiResponseJson: String): String
    @JvmStatic external fun nativeParseCrossSigningStatus(accountDataJson: String, userId: String): String
    @JvmStatic external fun nativeParseKeyBackupVersion(json: String): String

    // --- Device List ---

    @JvmStatic external fun nativeParseDeviceList(apiResponseJson: String, currentDeviceId: String): String

    // --- Room Permissions ---

    @JvmStatic external fun nativeComputePermissions(powerLevelsJson: String, myUserId: String): String

    // --- Room Tombstone ---

    @JvmStatic external fun nativeParseTombstone(contentJson: String): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeParseScanResult(apiResponseJson: String): String

    // --- Server Notice ---

    @JvmStatic external fun nativeParseServerNotice(eventContentJson: String, eventId: String): String

    // --- Member List ---

    @JvmStatic external fun nativeParseMemberList(roomId: String, apiResponseJson: String, isTruncated: Boolean): String

    // --- Public Room ---

    @JvmStatic external fun nativeParsePublicRoom(json: String): String

    // --- Event Relation Info ---

    @JvmStatic external fun nativeParseEventRelation(contentJson: String): String

    // --- Public Rooms / Thread ---

    @JvmStatic external fun nativeParsePublicRoomsResponse(json: String): String
    @JvmStatic external fun nativeComputeThreadSummary(rootEventId: String, eventsJson: String): String

    // --- Relation Description ---

    @JvmStatic external fun nativeFormatRelationDescription(relType: String, eventId: String, key: String): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeBuildScanRequestBody(mxcUri: String): String

    // --- Event Content ---

    @JvmStatic external fun nativeParseEventContent(eventType: String, contentJson: String): String

    // --- Canonical JSON ---

    @JvmStatic external fun nativeCanonicalizeJson(json: String): String

    // --- Chunked Uploader ---

    @JvmStatic external fun nativeUploaderSetChunkSizeMb(mb: Int)
    @JvmStatic external fun nativeUploaderComputeChunks(fileSize: Long): Int
    @JvmStatic external fun nativeUploaderGetChunkInfo(index: Int): String
    @JvmStatic external fun nativeUploaderContentRange(index: Int): String
    @JvmStatic external fun nativeUploaderAdvance()
    @JvmStatic external fun nativeUploaderCancel()
    @JvmStatic external fun nativeUploaderReset()
    @JvmStatic external fun nativeUploaderProgress(): String
    @JvmStatic external fun nativeSuggestChunkSizeMb(fileSize: Long): Int

    // --- Thread List ---

    @JvmStatic external fun nativeBuildThreadListJson(eventsJson: String): String

    // --- Thread Unread Counter ---

    @JvmStatic external fun nativeComputeThreadUnreadCount(eventIdsJson: String, readReceiptId: String, highlightIdsJson: String): String

    // --- Sync Filter ---

    @JvmStatic external fun nativeBuildSyncFilter(includeThreads: Boolean, includePresence: Boolean, timelineLimit: Int, lazyLoadMembers: Boolean): String

    // --- Bidi Text Security ---

    @JvmStatic external fun nativeContainsRtlOverride(text: String): Boolean
    @JvmStatic external fun nativeContainsBidiOverride(text: String): Boolean
    @JvmStatic external fun nativeFilterBidiOverrides(text: String): String
    @JvmStatic external fun nativeSanitizeDisplayText(text: String): String

    // --- Read Receipts ---

    @JvmStatic external fun nativeFormatReceiptAccessibility(receiptsJson: String, overflowCount: Int): String
    @JvmStatic external fun nativeFormatOverflowLabel(count: Int): String

    // --- Space Hierarchy ---

    @JvmStatic external fun nativeSearchSpaceChildren(childrenJson: String, query: String): String

    // --- 3PID / Presence ---

    @JvmStatic external fun nativeParseThreePid(input: String): String
    @JvmStatic external fun nativeFormatPresenceAggregation(userNamesJson: String, maxNames: Int): String

    // --- Reaction / Poll ---

    @JvmStatic external fun nativeFormatReactionAggregation(key: String, count: Int, reactorsJson: String): String
    @JvmStatic external fun nativeTrackPollResponse(optionId: String, userId: String): String

    // --- Audio / Voice ---

    @JvmStatic external fun nativeFormatPositionInfo(positionMs: Long, durationMs: Long): String

    // --- Direct Messages ---

    @JvmStatic external fun nativeParseDirectMessageMap(json: String): String

    // --- Edit History ---

    @JvmStatic external fun nativeGetEditCountBadge(editCount: Int): String
    @JvmStatic external fun nativeComputeEditDiffSummary(oldBody: String, newBody: String): String

    // --- Notification State ---

    @JvmStatic external fun nativeComputeNotificationState(roomJson: String): String

    // --- Room List Search ---

    @JvmStatic external fun nativeSearchRoomList(roomsJson: String, query: String): String

    // --- Event Classifier ---

    @JvmStatic external fun nativeIsStateEvent(eventType: String): Boolean

    // --- Poll Results ---

    @JvmStatic external fun nativeComputePollResults(pollJson: String): String

    // --- Location Sharing ---

    @JvmStatic external fun nativeLocationStartSession(sessionId: String, roomId: String, userId: String, intervalSec: Int): String
    @JvmStatic external fun nativeLocationStopSession(sessionId: String)
    @JvmStatic external fun nativeLocationIsDue(sessionId: String): Boolean
    @JvmStatic external fun nativeLocationExportJson(): String

    // --- AI Agent ---

    @JvmStatic external fun nativeAgentHasToolCalls(llmResponse: String): Boolean
    @JvmStatic external fun nativeAgentExtractTextAnswer(llmResponse: String): String
    @JvmStatic external fun nativeAgentGetToolsSchema(): String

    // --- Notification Formatting ---

    @JvmStatic external fun nativeFormatThreadNotificationCount(threadCount: Int, highlightCount: Int): String
    @JvmStatic external fun nativeFormatUnreadCounter(count: Int): String

    // --- Push Notification Evaluator ---

    @JvmStatic external fun nativeEvaluatePushNotification(eventJson: String, rulesJson: String, myDisplayName: String, myUserId: String): String

    // --- Room Upgrade ---

    @JvmStatic external fun nativeProcessRoomUpgrade(tombstoneEventJson: String): String

    // --- Redaction ---

    @JvmStatic external fun nativeFormatRedactionNotice(reason: String, redactedBySelf: Boolean, isStateEvent: Boolean): String

    // --- Key Backup ---

    @JvmStatic external fun nativeValidateAndFormatRecoveryKey(rawKey: String): String

    // --- Member / Call Notices ---

    @JvmStatic external fun nativeFormatMemberNotice(membership: String, prevMembership: String, senderId: String, senderName: String, targetId: String, targetName: String, reason: String, isDirect: Boolean, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatCallNotice(eventType: String, isVideo: Boolean, senderName: String, sentBySelf: Boolean): String
    @JvmStatic external fun nativeAnnotateEdited(body: String, isEdited: Boolean): String

    // --- Room State Notices ---

    @JvmStatic external fun nativeFormatRoomNameNotice(senderName: String, newName: String, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomTopicNotice(senderName: String, newTopic: String, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomAvatarNotice(senderName: String, isRemoved: Boolean, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomCreateNotice(senderName: String, predecessorRoomId: String, isDirect: Boolean, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomTombstoneNotice(senderName: String, replacementRoom: String, sentBySelf: Boolean): String
    @JvmStatic external fun nativeFormatRoomEncryptionNotice(senderName: String, isEnabled: Boolean, sentBySelf: Boolean): String

    // --- Power Level Diff ---

    @JvmStatic external fun nativeFormatPowerLevelDiff(senderName: String, oldLevelsJson: String, newLevelsJson: String, userNamesJson: String, sentBySelf: Boolean): String

    // --- Poll Validation ---

    @JvmStatic external fun nativeIsValidPollQuestion(question: String): Boolean

    // --- Room Uploads ---

    @JvmStatic external fun nativeIsStickerEvent(eventType: String): Boolean
    @JvmStatic external fun nativeHasAttachmentUrl(decryptedContentJson: String): Boolean
    @JvmStatic external fun nativeCreateUploadsFilterJson(numberOfEvents: Int): String

    // --- Matrix Error ---

    @JvmStatic external fun nativeGetErrorDescription(errorCode: String): String
    @JvmStatic external fun nativeIsPasswordError(errorCode: String): Boolean
    @JvmStatic external fun nativeGetAllErrorCodes(): String
    @JvmStatic external fun nativeIsRateLimitError(errorJson: String): Boolean
    @JvmStatic external fun nativeGetRetryAfterMs(errorJson: String): Long
    @JvmStatic external fun nativeIsSoftLogout(errorJson: String): Boolean
    @JvmStatic external fun nativeNeedsConsent(errorJson: String): Boolean
    @JvmStatic external fun nativeIsUserDeactivated(errorJson: String): Boolean

    // --- Error Classification (login/auth flow) ---

    @JvmStatic external fun nativeErrorIsTokenError(errorCode: String): Boolean
    @JvmStatic external fun nativeErrorShouldBeRetried(errorCode: String, httpCode: Int, isNetworkError: Boolean): Boolean
    @JvmStatic external fun nativeErrorIsInvalidUsername(errorCode: String, errorMessage: String): Boolean
    @JvmStatic external fun nativeErrorIsInvalidPassword(errorCode: String, errorMessage: String): Boolean
    @JvmStatic external fun nativeErrorIsWeakPassword(errorCode: String): Boolean
    @JvmStatic external fun nativeErrorIsLoginEmailUnknown(errorCode: String, errorMessage: String): Boolean
    @JvmStatic external fun nativeErrorIsHomeserverUnavailable(isNetworkError: Boolean, isUnknownHost: Boolean): Boolean
    @JvmStatic external fun nativeErrorIsRegistrationAvailability(errorCode: String, httpCode: Int): Boolean
    @JvmStatic external fun nativeClassifyError(errorCode: String, httpCode: Int, errorMessage: String, isNetworkError: Boolean, isUnknownHost: Boolean): String

    // --- Server ACL ---

    @JvmStatic external fun nativeWildcardMatch(pattern: String, value: String): Boolean
    @JvmStatic external fun nativeIsServerAllowed(serverName: String, aclJson: String): Boolean
    @JvmStatic external fun nativeCanJoinRoom(joinRulesJson: String, isInvited: Boolean, isMember: Boolean, isMemberOfAllowedRoom: Boolean): Boolean

    // --- Notification Formatter ---

    @JvmStatic external fun nativeFormatImageNotification(senderName: String): String
    @JvmStatic external fun nativeFormatFileNotification(senderName: String, fileName: String): String
    @JvmStatic external fun nativeFormatVideoNotification(senderName: String): String
    @JvmStatic external fun nativeFormatAudioNotification(senderName: String, isVoice: Boolean): String
    @JvmStatic external fun nativeFormatInviteNotification(inviterName: String, roomName: String): String
    @JvmStatic external fun nativeFormatRoomNotification(messageCount: Int, roomName: String): String
    @JvmStatic external fun nativeFormatStickerNotification(senderName: String): String
    @JvmStatic external fun nativeFormatLocationNotification(senderName: String): String
    @JvmStatic external fun nativeFormatPollNotification(senderName: String, isStart: Boolean): String

    // --- Raw Service ---

    @JvmStatic external fun nativeCacheKeyForUrl(url: String): String

    // --- Lightweight Settings ---

    @JvmStatic external fun nativeGetSettingBool(settingsJson: String, key: String, defaultVal: Boolean): Boolean
    @JvmStatic external fun nativeSetSettingBool(settingsJson: String, key: String, value: Boolean): String
    @JvmStatic external fun nativeGetSettingString(settingsJson: String, key: String, defaultVal: String): String
    @JvmStatic external fun nativeSetSettingString(settingsJson: String, key: String, value: String): String

    // --- HTTP Client ---

    @JvmStatic external fun nativeParseUrl(url: String): String

    // --- Account Export ---

        userId: String, token: String, refreshToken: String,
        homeServer: String, deviceId: String, deviceName: String,
        displayName: String, avatarUrl: String,
        includeCache: Boolean, passphrase: String
    ): String


    // --- Audio ---

    @JvmStatic external fun nativeFormatDuration(ms: Long): String

    // --- Media Filter ---


    // --- Content Filter ---


        blockAll: Boolean, allowAvatars: Boolean, allowStickers: Boolean, allowEmoji: Boolean,
        mxcUrl: String, imageType: String
    ): Boolean

    // --- Network Stats ---


    // --- Masquerade ---


    // --- User Mask ---


    // --- Chunked Upload ---


    // --- Chat Features (Timezone + EXIF) ---


    // --- Invitation Hide ---


    // --- Thread Aggregator ---

        threadId: String, roomId: String, roomName: String,
        accountId: String, accountIndex: String,
        lastMsg: String, lastSender: String,
        lastTs: Long, replyCount: Int, unread: Boolean
    )

    // --- User Messages ---


    // --- Room Version ---


    // --- Chat Preview ---


    // --- RAM Monitor ---


    // --- Cache Manager ---

    // --- Message Aggregator (All Messages) ---


    // --- Room Info ---


    // --- Deleted Archive ---


    // --- Search Index ---


    // --- Module Loader ---


    // --- Notification Keywords ---


    // --- Reaction Preview ---


    // --- Room Mirror ---


    // --- Input Tools ---



    // --- LLM ---

    @JvmStatic external fun nativeBuildLlmRequest(prompt: String, provider: Int, endpoint: String, token: String, model: String, systemPrompt: String, temp: Float, maxTokens: Int): String
    @JvmStatic external fun nativeBuildLlmHeaders(provider: Int, token: String): String
    @JvmStatic external fun nativeParseLlmResponse(body: String, statusCode: Int, provider: Int): String
    @JvmStatic external fun nativeFormatLlmBroadcast(prompt: String, response: String): String

    // --- Room Summary ---



    // --- Smart Reply ---


    // --- Weather Utils ---

    @JvmStatic external fun nativeWeatherBuildUrl(location: String, apiToken: String): String
    @JvmStatic external fun nativeWeatherParseWttr(json: String): String
    @JvmStatic external fun nativeWeatherParseOwm(json: String): String

    // --- Alarm Engine ---

    @JvmStatic external fun nativeAlarmCreate(agentText: String): String
    @JvmStatic external fun nativeAlarmGetNext(): String
    @JvmStatic external fun nativeAlarmListAll(): String
    @JvmStatic external fun nativeAlarmSnooze(id: String, minutes: Int)
    @JvmStatic external fun nativeAlarmDismiss(id: String)
    @JvmStatic external fun nativeAlarmDelete(id: String)
    @JvmStatic external fun nativeAlarmSetRingtone(id: String, uri: String)
    @JvmStatic external fun nativeAlarmLoad(json: String)


    @JvmStatic external fun nativeTextStats(text: String): String

    // --- Message Scheduler ---

    @JvmStatic external fun nativeSchedSchedule(roomId: String, body: String, formatted: String, triggerMs: Long): String
    @JvmStatic external fun nativeSchedCancel(id: String)
    @JvmStatic external fun nativeSchedGetPending(): String
    @JvmStatic external fun nativeSchedGetForRoom(roomId: String): String
    @JvmStatic external fun nativeSchedLoad(json: String)
    @JvmStatic external fun nativeSchedCleanup()

    // --- Notification Mode (Night Mode) ---

    @JvmStatic external fun nativeNotifSetMode(mode: Int)
    @JvmStatic external fun nativeNotifGetMode(): Int
    @JvmStatic external fun nativeNotifShouldPing(body: String, sender: String, isRoomPing: Boolean, isAlarm: Boolean): Boolean
    @JvmStatic external fun nativeNotifAddKeyword(keyword: String)
    @JvmStatic external fun nativeNotifRemoveKeyword(keyword: String)
    @JvmStatic external fun nativeNotifExport(): String
    @JvmStatic external fun nativeNotifLoad(json: String)

    // --- Duplicate Names ---


    // --- MXID Visibility ---


    // --- Read Receipts ---


    // --- Room Analytics ---


    // --- User Hide Timer ---


    // --- Message Queue ---


    // --- Image Crop ---


    // --- Auto-Scroll ---


    // --- Language Detection ---


    // --- Language Hide ---


    // --- Chat Push Down ---


    // --- Emoji Blacklist ---


    // --- Avatar History ---


    // --- Jump to Date with Time ---


    // --- Room Matching ---

    @JvmStatic external fun nativeIsRoomId(input: String): Boolean
    @JvmStatic external fun nativeIsRoomAlias(input: String): Boolean

    // --- Event Links ---

    @JvmStatic external fun nativeIsEventId(text: String): Boolean

    // --- Timestamps ---


    // --- Lightweight Call ---


    // --- Scheduled Edits ---


    // --- SVG Rendering ---


    // --- Drawing Canvas ---


    // --- Profile Swiper ---


    // --- Rainbow Generator ---


    // --- Text Formatting ---


    // --- URL Tools ---


    // --- Notification Priority ---


    // --- Matrix Patterns ---

    @JvmStatic external fun nativeIsUserId(input: String): Boolean
    @JvmStatic external fun nativeParseMatrixToPermalink(url: String): String
    @JvmStatic external fun nativeIsValidEmail(input: String): Boolean

    // --- Desync Detector ---


    // --- Latency Tracker ---


    // --- String Utils ---


    // --- Location Sharing ---


    // --- Color Utils ---


    // --- E2EE Utils ---

    @JvmStatic external fun nativeGetTrustLabel(level: Int): String

    // --- Thumbnail ---


    // --- Waveform ---


    // --- Session Timeout ---


    // --- Password Validator ---

    @JvmStatic external fun nativeValidatePassword(password: String): String

    // --- Spellcheck ---


    // --- Typing Indicator ---


    // --- Hash Utils ---


    // --- Room Stats ---


    // --- Mention Parser ---


    // --- Poll Utils ---
    // --- Reaction Utils ---


    // --- File Validator ---

    @JvmStatic external fun nativeFormatFileSize(bytes: Long): String

    // --- Date Utils ---


    // --- Message Queue ---


    // --- Pinned Events (Element Web parity) ---


    // --- Server Capabilities ---


    // --- Username Validator ---


    // --- Emoji Analyzer ---


    // --- Identity Utils ---

    @JvmStatic external fun nativeGetInitials(name: String): String

    // --- Notification Analyzer ---


    // --- Sync Analyzer ---


    // --- User Rating ---


    // --- Event Timeline ---


    // --- Room Directory ---


    // --- SSO Utils ---


    // --- Backup Utils ---


    // --- Device Manager ---

    @JvmStatic external fun nativeFormatDeviceLastSeen(lastSeenMs: Long): String
    @JvmStatic external fun nativeIsDeviceInactive(lastSeenMs: Long): Boolean

    // --- Presence ---

    @JvmStatic external fun nativeFormatPresence(presence: Int): String

    // --- Room Permissions ---


    // --- Room Summary ---


    // --- Membership ---

    @JvmStatic external fun nativeFormatMembership(membershipStr: String): String

    // --- Event Validator ---


    // --- Room Encryption ---


    // --- Login Utils ---


    // --- Account Utils ---


    // --- Connection Monitor ---

    @JvmStatic external fun nativeConnMonitorOnReconnectAttempt()

    // --- Push Rules ---


    // --- Space Utils ---

    @JvmStatic external fun nativeBuildSpaceChildContent(suggested: Boolean, order: String, autoJoin: Boolean, canonical: Boolean): String

    // --- Event Relations ---
    // --- E2EE Decoration ---


    // --- Room List ---


    // --- Media Utils ---


    // --- Notification Settings ---

    @JvmStatic external fun nativeFormatNotifMode(mode: Int): String

    // --- Invite Utils ---

    @JvmStatic external fun nativeBuildInviteBody(userId: String, reason: String): String

    // --- Verification ---


    // --- Session Manager ---


    // --- Auth Utils ---


    // --- Content Scanner ---

    @JvmStatic external fun nativeIsServerNotice(contentJson: String): Boolean

    // --- Event Encryption ---


    // --- Report Utils ---

    @JvmStatic external fun nativeGetReasonDescription(code: String): String

    // --- Secret Storage ---

    @JvmStatic external fun nativeExtractDefaultSecretKey(accountDataJson: String): String
    @JvmStatic external fun nativeHasCrossSigningSecrets(accountDataJson: String): Boolean

    // --- Report / Rageshake ---

    @JvmStatic external fun nativeIsOffensive(score: Int): Boolean
    @JvmStatic external fun nativeTruncateReportDescription(description: String, maxLen: Int): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeIsContentScannerAvailable(serverCapabilitiesJson: String): Boolean

    // --- Matrix Error ---
    // --- Call Content Builders ---

    @JvmStatic external fun nativeBuildCallInviteContent(callId: String, isVideo: Boolean, sdpOffer: String, lifetimeSec: Int): String
    @JvmStatic external fun nativeBuildCallAnswerContent(callId: String, sdpAnswer: String): String
    @JvmStatic external fun nativeGetCallState(eventContentJson: String): String

    // --- Room State ---

    @JvmStatic external fun nativeIsPublicRoom(stateContentJson: String): Boolean
    @JvmStatic external fun nativeIsInviteOnly(stateContentJson: String): Boolean
    @JvmStatic external fun nativeJoinRuleToString(stateContentJson: String): String
    @JvmStatic external fun nativeIsHistoryPubliclyVisible(stateContentJson: String): Boolean
    @JvmStatic external fun nativeHistoryVisibilityToString(stateContentJson: String): String
    @JvmStatic external fun nativeAreGuestsAllowed(stateContentJson: String): Boolean
    @JvmStatic external fun nativeIsRoomUpgraded(stateContentJson: String): Boolean

    // --- Matrix Pattern Validators ---

    @JvmStatic external fun nativeIsMxcUrl(url: String): Boolean
    @JvmStatic external fun nativeIsPhoneNumber(input: String): Boolean
    @JvmStatic external fun nativeExtractUserNameFromId(mxid: String): String
    @JvmStatic external fun nativeCandidateAliasFromRoomName(roomName: String, domain: String, maxLength: Int): String

    // --- Widget List ---

    @JvmStatic external fun nativeListRoomWidgets(stateEventsJson: String): String

    // --- Session Rename ---

    @JvmStatic external fun nativeBuildSessionRenameBody(sessionId: String, newName: String): String

    // --- Permalink Detection ---

    @JvmStatic external fun nativeIsMatrixToPermalink(url: String): Boolean
    @JvmStatic external fun nativeIsAppPermalink(url: String): Boolean
    @JvmStatic external fun nativeIsValidOrderString(order: String): Boolean
    @JvmStatic external fun nativeIsGroupId(input: String): Boolean

    // --- Device Name ---

    @JvmStatic external fun nativeParseDeviceName(userAgent: String): String

    // --- Matrix ID Extraction ---

    @JvmStatic external fun nativeExtractMatrixIds(text: String): String

    // --- Login Flows ---

    @JvmStatic external fun nativeParseLoginFlowsList(apiResponseJson: String): String
    @JvmStatic external fun nativeBuildUserIdentifier(userId: String): String

    // --- Notification Mode ---

    @JvmStatic external fun nativeIsNotifModeDifferent(oldMode: String, newMode: String): Boolean
    @JvmStatic external fun nativeGetDefaultModeForRoom(isDirect: Boolean, isEncrypted: Boolean): String

    // --- Password Strength ---

    @JvmStatic external fun nativeMeetsMinimumRequirements(password: String): Boolean
    @JvmStatic external fun nativeCountCharClasses(password: String): Int
    @JvmStatic external fun nativeIsCommonPassword(password: String): Boolean

    // --- SSO Utilities ---

    @JvmStatic external fun nativeBuildSsoLoginUrl(baseUrl: String, redirectUrl: String): String
    @JvmStatic external fun nativeGetSsoProviderBrand(provider: String): String

    // --- Trust Label ---
    // --- MXC URL Utilities ---

    @JvmStatic external fun nativeIsMxcUri(url: String): Boolean
    @JvmStatic external fun nativeExtractMxcServerName(mxcUrl: String): String
    @JvmStatic external fun nativeExtractMxcMediaId(mxcUrl: String): String
    @JvmStatic external fun nativeBuildMxcUri(serverName: String, mediaId: String): String
    @JvmStatic external fun nativeResolveMxcDownloadUrl(mxcUrl: String, homeServerUrl: String): String
    @JvmStatic external fun nativeHasTextWithImage(contentJson: String): Boolean

    // --- MXC Thumbnail ---

    @JvmStatic external fun nativeResolveMxcThumbnailUrl(mxcUrl: String, homeServerUrl: String, width: Int, height: Int): String

    // --- Content Utilities ---

    @JvmStatic external fun nativeGetExtensionFromMimeType(mimetype: String): String
    @JvmStatic external fun nativeExtractUsefulTextFromReply(repliedBody: String): String
    @JvmStatic external fun nativeGetLatestEditEventId(editSummaryJson: String, originalEventId: String): String
    @JvmStatic external fun nativeGetEditedTargetEventId(contentJson: String): String

    // --- User Status ---

    @JvmStatic external fun nativeBuildUserStatusJson(status: String, emoji: String, nowMs: Long): String
    @JvmStatic external fun nativeGetPresenceStatusText(isOnline: Boolean, lastActiveMs: Long): String
    @JvmStatic external fun nativeGetStatusSuggestions(): String

    // --- Markdown Renderer ---
    // --- Megolm Decryptor ---

    @JvmStatic external fun nativeMegolmAddSession(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String): Boolean
    @JvmStatic external fun nativeMegolmDecrypt(roomId: String, senderKey: String, sessionId: String, ciphertext: String): String
    @JvmStatic external fun nativeMegolmSessionCount(): Int
    @JvmStatic external fun nativeMegolmClearRoom(roomId: String)

    // --- Olm Account & Session ---

    @JvmStatic external fun nativeOlmCreateAccount(userId: String, deviceId: String): Boolean
    @JvmStatic external fun nativeOlmGetIdentityKeys(): String
    @JvmStatic external fun nativeOlmGenerateOneTimeKeys(count: Int): String
    @JvmStatic external fun nativeOlmSignMessage(message: String): String
    @JvmStatic external fun nativeOlmCreateInboundSession(theirIdentityKey: String, preKeyMessage: String): String
    @JvmStatic external fun nativeOlmDecryptMessage(senderKey: String, sessionId: String, ciphertext: String): String
    @JvmStatic external fun nativeOlmPickleAccount(): String
    @JvmStatic external fun nativeOlmUnpickleAccount(pickled: String, userId: String, deviceId: String): Boolean

    // --- Event Signing ---

    @JvmStatic external fun nativeSignEvent(eventJson: String): String
    @JvmStatic external fun nativeVerifyEventSignature(eventJson: String, signKeyB64: String): Boolean

    // --- Device Verification ---

    @JvmStatic external fun nativeVerifyDeviceSignature(deviceKeysJson: String, userId: String, deviceId: String, signKeyB64: String, signatureB64: String): Boolean
    @JvmStatic external fun nativeComputeDeviceFingerprint(identityKeyBase64: String): String

    // --- SAS Emoji Verification ---

    @JvmStatic external fun nativeSasCreate(): String
    @JvmStatic external fun nativeSasSetTheirKey(theirPubkey: String): Boolean
    @JvmStatic external fun nativeSasGetEmojis(): String
    @JvmStatic external fun nativeSasCalculateMac(input: String, info: String): String
    @JvmStatic external fun nativeSasVerifyMac(theirMac: String, input: String, info: String): Boolean
    @JvmStatic external fun nativeSasDestroy()

    // --- JSON Parser ---

    @JvmStatic external fun nativeParseJsonStringValue(json: String, key: String): String

    // --- Federation Version ---

    @JvmStatic external fun nativeFederationVersionToJson(versionJson: String): String

    // --- Auth Models ---

    @JvmStatic external fun nativePresenceEnumToString(presence: Int): String
    @JvmStatic external fun nativeCredentialsToJson(credentialsJson: String): String

    // --- Call Models ---

    @JvmStatic external fun nativeSdpTypeToString(type: Int): String
    @JvmStatic external fun nativeEndCallReasonToString(reason: Int): String

    // --- Message Content ---

    @JvmStatic external fun nativeMessageTextToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageNoticeToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageEmoteToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageImageToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageVideoToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageAudioToJson(contentJson: String): String
    @JvmStatic external fun nativeMessageFileToJson(contentJson: String): String

    // --- Crypto Models ---

    @JvmStatic external fun nativeDeviceInfoToJson(deviceJson: String): String

    // --- Offline Cache ---

    @JvmStatic external fun nativeCanFitInStorage(required: Long, available: Long, reserved: Long): Boolean
    @JvmStatic external fun nativeEstimateMessageCacheSize(messageCount: Int, avgBodySize: Int): Long

    // --- Sign Out Service ---

    @JvmStatic external fun nativeShouldIgnoreSignOutError(errorCode: String, httpCode: Int): Boolean
    @JvmStatic external fun nativeSignInAgainBodyToJson(paramsJson: String): String

    // --- Message Extras ---

    @JvmStatic external fun nativePollTypeToString(type: Int): String
    @JvmStatic external fun nativePollTypeFromString(type: String): Int

    // --- Terms Service ---

    @JvmStatic external fun nativeAcceptTermsBodyToJson(bodyJson: String): String

    // --- Live Draft ---

    @JvmStatic external fun nativeLiveDraftConfigToJson(configJson: String): String

    // --- Encrypted File ---

    @JvmStatic external fun nativeEncryptedFileKeyToJson(keyJson: String): String
    @JvmStatic external fun nativeIsValidJwkKey(keyJson: String): Boolean
    @JvmStatic external fun nativeExtractFileKey(keyJson: String): String
    @JvmStatic external fun nativeEncryptedFileInfoToJson(infoJson: String): String
    @JvmStatic external fun nativeIsValidEncryptedFile(infoJson: String): Boolean
    @JvmStatic external fun nativeExtractFileIv(infoJson: String): String

    // --- Crypto Algorithms ---

    @JvmStatic external fun nativeSha256(data: ByteArray): String
    @JvmStatic external fun nativeBase58Encode(data: ByteArray): String

    // --- TLS Bridge ---

    @JvmStatic external fun nativeTlsBridgeAvailable(): Boolean

    // --- Create Room ---

    @JvmStatic external fun nativeCreateRoomPresetToString(preset: Int): String

    // --- Widget Manager ---

    @JvmStatic external fun nativeWidgetMgrInit(roomId: String, userId: String, displayName: String, avatarUrl: String): Boolean
    @JvmStatic external fun nativeWidgetMgrSetSecurityPolicy(policyJson: String): Boolean
    @JvmStatic external fun nativeWidgetMgrLoadWidgets(stateEventsJson: String): String
    @JvmStatic external fun nativeWidgetMgrCreateWidget(widgetId: String, type: String, url: String, name: String, waitForIframeLoad: Boolean): String
    @JvmStatic external fun nativeWidgetMgrRemoveWidget(widgetId: String): String
    @JvmStatic external fun nativeWidgetMgrSetPinned(widgetId: String, pinned: Boolean): String
    @JvmStatic external fun nativeWidgetMgrResize(widgetId: String, width: Int, height: Int): String
    @JvmStatic external fun nativeWidgetMgrSetMinimized(widgetId: String, minimized: Boolean): String
    @JvmStatic external fun nativeWidgetMgrSetMaximized(widgetId: String, maximized: Boolean): String
    @JvmStatic external fun nativeWidgetMgrRequestCapability(widgetId: String, capability: Int): String
    @JvmStatic external fun nativeWidgetMgrApproveCapability(widgetId: String, capability: Int): String
    @JvmStatic external fun nativeWidgetMgrDenyCapability(widgetId: String, capability: Int): String
    @JvmStatic external fun nativeWidgetMgrGetUrl(widgetId: String): String
    @JvmStatic external fun nativeWidgetMgrBuildPostMessage(widgetId: String, action: String, data: String): String
    @JvmStatic external fun nativeWidgetMgrParsePostMessage(message: String): String
    @JvmStatic external fun nativeWidgetMgrSupportsPiP(widgetId: String): Boolean
    @JvmStatic external fun nativeWidgetMgrGetByType(type: String): String
    @JvmStatic external fun nativeWidgetMgrCount(): String
    @JvmStatic external fun nativeWidgetMgrBuildCsp(): String
    @JvmStatic external fun nativeApplyWidgetUrlTemplate(url: String, templateJson: String): String
    @JvmStatic external fun nativeValidateWidgetSecurity(url: String, policyJson: String): String
    @JvmStatic external fun nativeClassifyWidgetType(type: String): String
    @JvmStatic external fun nativeIsAutoApprovedCapability(capability: Int, widgetType: String): Boolean

    // --- Key Backup Manager ---

    @JvmStatic external fun nativeBackupExtractPrivateKey(recoveryKey: String): String
    @JvmStatic external fun nativeBackupGenerateRecoveryKey(curve25519Key: String): String
    @JvmStatic external fun nativeBackupParseVersion(json: String): String
    @JvmStatic external fun nativeBackupBuildCreateVersion(configJson: String): String
    @JvmStatic external fun nativeBackupBuildDelete(version: String): String
    @JvmStatic external fun nativeBackupExportSession(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String, firstMessageIndex: Long, isForwarded: Boolean, forwardedCount: Long): String
    @JvmStatic external fun nativeBackupEncryptSession(sessionJson: String, authData: String): String
    @JvmStatic external fun nativeBackupParseKeys(backupJson: String): String
    @JvmStatic external fun nativeBackupDecryptSession(sessionJson: String, backupKey: String, roomId: String): String
    @JvmStatic external fun nativeBackupDecryptAll(keysJson: String, authData: String, recoveryKey: String): String
    @JvmStatic external fun nativeBackupVerifyIntegrity(authData: String): Boolean
    @JvmStatic external fun nativeBackupVerifyRecoveryMatch(recoveryKey: String, authData: String): Boolean
    @JvmStatic external fun nativeBackupProgress(): String
    @JvmStatic external fun nativeBackupProgressJson(): String
    @JvmStatic external fun nativeBackupSetTotalKeys(count: Int)
    @JvmStatic external fun nativeBackupAdvanceUploaded()
    @JvmStatic external fun nativeBackupAdvanceDownloaded()
    @JvmStatic external fun nativeBackupAdvanceDecrypted()
    @JvmStatic external fun nativeBackupAdvanceImported()
    @JvmStatic external fun nativeBackupMarkComplete()
    @JvmStatic external fun nativeBackupReset()

    // --- Live Location ---

    @JvmStatic external fun nativeLiveLocationParseGeoUri(uri: String): String
    @JvmStatic external fun nativeLiveLocationFormatMessage(lat: Double, lon: Double, accuracy: Double, label: String): String
    @JvmStatic external fun nativeLiveLocationFormatGeoUri(lat: Double, lon: Double): String
    @JvmStatic external fun nativeLiveLocationDistance(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double
    @JvmStatic external fun nativeLiveLocationStartSession(roomId: String, userId: String, description: String, timeoutSec: Int, intervalSec: Int, autoStop: Boolean, autoStopMin: Int): String
    @JvmStatic external fun nativeLiveLocationStopSession(sessionId: String): String
    @JvmStatic external fun nativeLiveLocationIsDue(sessionId: String): Boolean
    @JvmStatic external fun nativeLiveLocationUpdate(sessionId: String, lat: Double, lon: Double, accuracy: Double): String
    @JvmStatic external fun nativeLiveLocationGetActive(userId: String): String
    @JvmStatic external fun nativeLiveLocationGetRoomSessions(roomId: String): String
    @JvmStatic external fun nativeLiveLocationHistory(sessionId: String): String
    @JvmStatic external fun nativeLiveLocationBuildMapUrl(roomId: String, configJson: String): String
    @JvmStatic external fun nativeLiveLocationCluster(coordsJson: String, radiusMeters: Double): String
    @JvmStatic external fun nativeLiveLocationWithinGeofence(lat: Double, lon: Double, centerLat: Double, centerLon: Double, radiusMeters: Double): Boolean

    // --- Call Manager ---

    @JvmStatic external fun nativeCallStartOutgoing(roomId: String, calleeId: String, calleeName: String, callType: Int, sdpOffer: String): String
    @JvmStatic external fun nativeCallHandleIncoming(callId: String, roomId: String, callerId: String, callerName: String, callType: Int, sdpOffer: String, lifetimeSec: Int): String
    @JvmStatic external fun nativeCallAnswer(callId: String, sdpAnswer: String): String
    @JvmStatic external fun nativeCallReject(callId: String): String
    @JvmStatic external fun nativeCallHangup(callId: String): String
    @JvmStatic external fun nativeCallGetActive(): String
    @JvmStatic external fun nativeCallGetIncoming(): String
    @JvmStatic external fun nativeCallGetRoomCalls(roomId: String): String
    @JvmStatic external fun nativeCallIsRoomInCall(roomId: String): Boolean
    @JvmStatic external fun nativeCallFormatDuration(seconds: Int): String
    @JvmStatic external fun nativeCallParseSdp(sdpText: String, type: String): String
    @JvmStatic external fun nativeCallSetMuted(callId: String, muted: Boolean)
    @JvmStatic external fun nativeCallSetVideo(callId: String, enabled: Boolean)
    @JvmStatic external fun nativeCallReset()

    // --- Thread Manager ---

    @JvmStatic external fun nativeThreadIsRoot(eventContent: String, eventId: String): Boolean
    @JvmStatic external fun nativeThreadExtractRoot(eventContent: String): String
    @JvmStatic external fun nativeThreadUpsert(threadJson: String)
    @JvmStatic external fun nativeThreadGetList(limit: Int, offset: Int): String
    @JvmStatic external fun nativeThreadSetUnread(threadId: String, count: Int, highlighted: Boolean)
    @JvmStatic external fun nativeThreadMarkRead(threadId: String, readPos: Long)
    @JvmStatic external fun nativeThreadGetUnreadState(threadId: String): String
    @JvmStatic external fun nativeThreadTotalUnread(): Int
    @JvmStatic external fun nativeThreadFormatCount(count: Int): String
    @JvmStatic external fun nativeThreadGetNotifications(): String
    @JvmStatic external fun nativeThreadReset()

    // --- Poll Manager ---

    @JvmStatic external fun nativePollBuildStart(question: String, optionsJson: String, kind: Int, maxSelections: Int, unstable: Boolean): String
    @JvmStatic external fun nativePollBuildResponse(pollId: String, selectionsJson: String, unstable: Boolean): String
    @JvmStatic external fun nativePollBuildEnd(pollId: String, reason: String, unstable: Boolean): String
    @JvmStatic external fun nativePollTally(pollJson: String, votesJson: String): String
    @JvmStatic external fun nativePollIsValidQuestion(question: String): Boolean

    // --- Space Graph ---

    @JvmStatic external fun nativeSpaceSetRoot(spaceId: String, name: String, topic: String, avatarUrl: String)
    @JvmStatic external fun nativeSpaceAddChildRaw(parentId: String, childId: String, suggested: Boolean)
    @JvmStatic external fun nativeSpaceAddChild(parentId: String, childJson: String)
    @JvmStatic external fun nativeSpaceSetMetadata(roomId: String, name: String, topic: String, avatarUrl: String, joinRule: String, isJoined: Boolean)
    @JvmStatic external fun nativeSpaceTraverse(mode: Int, maxDepth: Int): String
    @JvmStatic external fun nativeSpaceGetChildren(spaceId: String): String
    @JvmStatic external fun nativeSpaceGetParents(roomId: String): String
    @JvmStatic external fun nativeSpaceGetDepth(roomId: String): Int
    @JvmStatic external fun nativeSpaceIsInSpace(spaceId: String, roomId: String): Boolean
    @JvmStatic external fun nativeSpaceToTree(spaceId: String, maxDepth: Int): String
    @JvmStatic external fun nativeSpaceSearch(spaceId: String, query: String): String
    @JvmStatic external fun nativeSpaceReset()

    // --- Pin Manager ---

    @JvmStatic external fun nativePinEvent(roomId: String, eventId: String, pinnedBy: String, powerLevel: Int): String
    @JvmStatic external fun nativeUnpinEvent(roomId: String, eventId: String, removedBy: String, powerLevel: Int): String
    @JvmStatic external fun nativePinToggle(roomId: String, eventId: String, userId: String, powerLevel: Int): String
    @JvmStatic external fun nativePinLoadState(roomId: String, stateJson: String)
    @JvmStatic external fun nativePinGetEvents(roomId: String): String
    @JvmStatic external fun nativePinIsPinned(roomId: String, eventId: String): Boolean
    @JvmStatic external fun nativePinCount(roomId: String): Int
    @JvmStatic external fun nativePinCanManage(powerLevel: Int): Boolean
    @JvmStatic external fun nativePinReset()

    // --- Media Viewer ---

    @JvmStatic external fun nativeMediaViewerParse(contentJson: String): String
    @JvmStatic external fun nativeMediaViewerFormatSize(bytes: Long): String
    @JvmStatic external fun nativeMediaViewerFormatDuration(durationMs: Int): String
    @JvmStatic external fun nativeMediaViewerViewport(contentJson: String, viewportW: Int, viewportH: Int): String
    @JvmStatic external fun nativeMediaViewerThumbnailUrl(mxcUrl: String, homeServer: String, width: Int, height: Int): String
    @JvmStatic external fun nativeMediaViewerDownloadUrl(mxcUrl: String, homeServer: String): String
    @JvmStatic external fun nativeMediaViewerExifRotation(rawExif: Int): Int
    @JvmStatic external fun nativeMediaViewerCanThumbnail(mimeType: String): Boolean

    // --- OIDC/SSO Login ---

    @JvmStatic external fun nativeOidcParseMetadata(json: String): String
    @JvmStatic external fun nativeOidcBuildRegistration(configJson: String): String
    @JvmStatic external fun nativeOidcParseRegistration(json: String): String
    @JvmStatic external fun nativeOidcBuildAuthorization(metadataJson: String, registrationJson: String, configJson: String): String
    @JvmStatic external fun nativeOidcParseToken(json: String): String
    @JvmStatic external fun nativeOidcBuildRefresh(refreshToken: String, clientId: String): String
    @JvmStatic external fun nativeOidcParseWhoami(json: String): String
    @JvmStatic external fun nativeOidcParseWellKnown(json: String): String
    @JvmStatic external fun nativeOidcIsCallback(url: String): Boolean
    @JvmStatic external fun nativeOidcExtractCode(callbackUrl: String): String
    @JvmStatic external fun nativeOidcBuildPasswordLogin(userId: String, password: String, deviceId: String, deviceName: String): String

    // --- User Directory ---

    @JvmStatic external fun nativeUserDirBuildSearch(searchTerm: String, limit: Int): String
    @JvmStatic external fun nativeUserDirSearch(query: String, responseJson: String): String
    @JvmStatic external fun nativeUserDirBestName(displayName: String, userId: String): String
    @JvmStatic external fun nativeUserDirAvatarInit(displayName: String, userId: String): String
    @JvmStatic external fun nativeUserDirIsValidQuery(query: String): Boolean

    // --- Profiler ---

    @JvmStatic external fun nativeProfileStart()
    @JvmStatic external fun nativeProfileStop()
    @JvmStatic external fun nativeProfileReset()
    @JvmStatic external fun nativeProfileIsActive(): Boolean
    @JvmStatic external fun nativeProfileReport(): String
    @JvmStatic external fun nativeProfileReportText(): String
    @JvmStatic external fun nativeProfileGetSummary(name: String): String
    @JvmStatic external fun nativeProfileMemory(): String

    // --- Profiler: User Action Timing ---
    @JvmStatic external fun nativeProfileStartAction(actionName: String, isCold: Boolean): Int
    @JvmStatic external fun nativeProfileStopAction(actionIndex: Int): Long
    @JvmStatic external fun nativeProfileSetBudget(actionPattern: String, budgetNs: Long)
    @JvmStatic external fun nativeProfileActionReport(): String
    @JvmStatic external fun nativeProfileActionReportText(): String

    // --- Profiler: Real-Time Overlay ---
    @JvmStatic external fun nativeProfileOverlaySnapshot(): String
    @JvmStatic external fun nativeProfileOverlayText(): String

    // --- Device Manager Full ---

    @JvmStatic external fun nativeDeviceParseList(json: String): String
    @JvmStatic external fun nativeDeviceParseInfo(deviceId: String, json: String): String
    @JvmStatic external fun nativeDeviceParseCrypto(deviceId: String, userId: String, json: String): String
    @JvmStatic external fun nativeDeviceBuildRename(deviceId: String, newName: String): String
    @JvmStatic external fun nativeDeviceBuildDelete(deviceId: String, authType: String, authSession: String, password: String): String
    @JvmStatic external fun nativeDeviceFormatFingerprint(rawKey: String): String
    @JvmStatic external fun nativeDeviceGetTrustLabel(crossSigningVerified: Boolean, locallyVerified: Boolean): String
    @JvmStatic external fun nativeDeviceFormatLastSeen(timestampMs: Long): String
    @JvmStatic external fun nativeDeviceIsInactive(lastSeenTs: Long, inactivityDays: Int): Boolean
    @JvmStatic external fun nativeDeviceSatisfiesVersion(clientVersion: String, minRequired: String): Boolean

    // --- Room Directory ---

    @JvmStatic external fun nativeRoomDirBuildSearch(searchTerm: String, limit: Int, since: String): String
    @JvmStatic external fun nativeRoomDirParseResponse(json: String): String
    @JvmStatic external fun nativeRoomDirBuildVisibility(visibility: Int): String
    @JvmStatic external fun nativeRoomDirParseVisibility(json: String): String
    @JvmStatic external fun nativeRoomDirCheckAlias(aliasLocalPart: String, json: String): String
    @JvmStatic external fun nativeRoomDirFormatPreview(roomJson: String): String

    // --- Session Manager (multi-account) ---

    @JvmStatic external fun nativeSessionComputeId(userId: String, deviceId: String): String
    @JvmStatic external fun nativeSessionCreate(credentialsJson: String, configJson: String, loginType: Int): String
    @JvmStatic external fun nativeSessionOpen(sessionId: String): Boolean
    @JvmStatic external fun nativeSessionClose(sessionId: String): Boolean
    @JvmStatic external fun nativeSessionRemove(sessionId: String): Boolean
    @JvmStatic external fun nativeSessionSetActive(sessionId: String): Boolean
    @JvmStatic external fun nativeSessionGetActive(): String
    @JvmStatic external fun nativeSessionHasActive(): Boolean
    @JvmStatic external fun nativeSessionGetAll(): String
    @JvmStatic external fun nativeSessionCount(): Int

    // --- Server Notice Handler ---

    @JvmStatic external fun nativeServerNoticeParse(errorJson: String): String
    @JvmStatic external fun nativeServerNoticeFormatLimit(errorJson: String, mode: Int): String
    @JvmStatic external fun nativeServerNoticeGetDescription(errorCode: String): String
    @JvmStatic external fun nativeServerNoticeIsResourceLimit(errorCode: String): Boolean
    @JvmStatic external fun nativeServerNoticeIsRateLimit(errorCode: String): Boolean
    @JvmStatic external fun nativeServerNoticeIsConsent(errorCode: String): Boolean
    @JvmStatic external fun nativeServerNoticeFormatDowntime(retryAfterMs: Long): String
    @JvmStatic external fun nativeServerNoticeGetBanner(errorJson: String): String

    // --- Media Upload Manager ---

    @JvmStatic external fun nativeUploadParseResponse(json: String): String
    @JvmStatic external fun nativeUploadBuildContent(attachmentJson: String, mxcUrl: String): String
    @JvmStatic external fun nativeUploadIsSizeValid(fileSize: Long): Boolean
    @JvmStatic external fun nativeUploadFormatSizeWarning(fileSize: Long, maxSize: Long): String
    @JvmStatic external fun nativeUploadGetProgress(): String
    @JvmStatic external fun nativeUploadResetProgress(totalBytes: Long)
    @JvmStatic external fun nativeUploadSetMaxSize(maxBytes: Long)

    // --- Identity Server Manager ---

    @JvmStatic external fun nativeIdentityParse3pid(input: String): String
    @JvmStatic external fun nativeIdentityBuildBind(threePid: String): String
    @JvmStatic external fun nativeIdentityBuildLookup(pidsJson: String): String
    @JvmStatic external fun nativeIdentityParseLookup(json: String): String
    @JvmStatic external fun nativeIdentitySetServer(url: String): String
    @JvmStatic external fun nativeIdentityGetServer(): String

    // --- Event Relations ---

    @JvmStatic external fun nativeRelationParse(eventContent: String): String
    @JvmStatic external fun nativeRelationIsReply(eventContent: String): Boolean
    @JvmStatic external fun nativeRelationIsEdit(eventContent: String): Boolean
    @JvmStatic external fun nativeRelationIsReaction(eventContent: String): Boolean
    @JvmStatic external fun nativeRelationExtractThreadRoot(eventContent: String): String
    @JvmStatic external fun nativeRelationExtractReplySource(eventContent: String): String
    @JvmStatic external fun nativeRelationBuildReply(eventId: String): String
    @JvmStatic external fun nativeRelationBuildEdit(eventId: String): String
    @JvmStatic external fun nativeRelationBuildThread(eventId: String, replyToId: String): String
    @JvmStatic external fun nativeRelationBuildAnnotation(eventId: String, key: String): String

    // --- Cross-Signing ---

    @JvmStatic external fun nativeCrossSigningIsInit(): Boolean
    @JvmStatic external fun nativeCrossSigningCanSign(): Boolean
    @JvmStatic external fun nativeCrossSigningBuildKeys(userId: String, mskPublic: String, uskPublic: String, sskPublic: String): String
    @JvmStatic external fun nativeCrossSigningCheckSelf(): String
    @JvmStatic external fun nativeCrossSigningImportKeys(mskPrivate: String, uskPrivate: String, sskPrivate: String): String
    @JvmStatic external fun nativeCrossSigningTrustMaster()

    // --- Draft Manager ---

    @JvmStatic external fun nativeDraftSave(roomId: String, content: String, type: Int, linkedEventId: String)
    @JvmStatic external fun nativeDraftGet(roomId: String): String
    @JvmStatic external fun nativeDraftDelete(roomId: String)
    @JvmStatic external fun nativeDraftHasDraft(roomId: String): Boolean
    @JvmStatic external fun nativeDraftAutoSave(roomId: String, text: String): Boolean
    @JvmStatic external fun nativeDraftStripPrefix(text: String): String

    // --- Room History Visibility ---

    @JvmStatic external fun nativeRoomStateParseVisibility(contentJson: String): String
    @JvmStatic external fun nativeRoomStateParseJoinRules(contentJson: String): String
    @JvmStatic external fun nativeRoomStateShouldShare(contentJson: String): Boolean
    @JvmStatic external fun nativeRoomStateIsPublic(roomId: String): Boolean
    @JvmStatic external fun nativeRoomStateIsInviteOnly(roomId: String): Boolean
    @JvmStatic external fun nativeRoomStateSetVisibility(roomId: String, visibility: Int)
    @JvmStatic external fun nativeRoomStateSetJoinRule(roomId: String, joinRule: Int)

    // --- Terms/Consent Manager ---

    @JvmStatic external fun nativeTermsParse(json: String): String
    @JvmStatic external fun nativeTermsBuildAgree(urlsJson: String): String
    @JvmStatic external fun nativeTermsAreRequired(errorJson: String): Boolean
    @JvmStatic external fun nativeTermsGetPending(responseJson: String, agreedJson: String): String

    // --- Transparent Overlay ---

    @JvmStatic external fun nativeOverlaySetConfig(configJson: String)
    @JvmStatic external fun nativeOverlayTouchDown(x: Double, y: Double, pointerId: Int, timeNs: Long): Int
    @JvmStatic external fun nativeOverlayTouchMove(x: Double, y: Double, pointerId: Int, timeNs: Long): Int
    @JvmStatic external fun nativeOverlayTouchUp(pointerId: Int, timeNs: Long): Int
    @JvmStatic external fun nativeOverlayBack(timeNs: Long): Int
    @JvmStatic external fun nativeOverlayTick(timeNs: Long): Int
    @JvmStatic external fun nativeOverlayGetState(): String

    // --- Transparent Overlay: Safety Mode ---
    @JvmStatic external fun nativeOverlaySetSafetyMode(mode: Int)
    @JvmStatic external fun nativeOverlaySetSafetyPerms(permissionsJson: String)
    @JvmStatic external fun nativeOverlayIsTouchAllowed(action: Int): Boolean
    @JvmStatic external fun nativeOverlaySafetyToJson(): String

    // --- Message Composer ---

    @JvmStatic external fun nativeComposerSetText(text: String)
    @JvmStatic external fun nativeComposerGetState(): String
    @JvmStatic external fun nativeComposerEnterRegular()
    @JvmStatic external fun nativeComposerEnterEdit(eventId: String)
    @JvmStatic external fun nativeComposerEnterQuote(eventId: String)
    @JvmStatic external fun nativeComposerEnterReply(eventId: String)
    @JvmStatic external fun nativeComposerApplyBold(text: String, selStart: Int, selEnd: Int): String
    @JvmStatic external fun nativeComposerApplyItalic(text: String, selStart: Int, selEnd: Int): String
    @JvmStatic external fun nativeComposerBuildQuoted(quotedText: String, replyText: String, quotedSender: String): String
    @JvmStatic external fun nativeComposerAutoEmoji(text: String): String
    @JvmStatic external fun nativeComposerExtractMention(text: String, cursorPos: Int): String
    @JvmStatic external fun nativeComposerValidate(text: String, maxLength: Int): String

    // --- Text Undo (delete protection) ---

    @JvmStatic external fun nativeUndoSetConfig(configJson: String)
    @JvmStatic external fun nativeUndoCheckpoint(text: String, cursorPos: Int, description: String)
    @JvmStatic external fun nativeUndoOnSelectAll(text: String, cursorPos: Int)
    @JvmStatic external fun nativeUndoOnBeforePaste(currentText: String, cursorPos: Int, pastedText: String)
    @JvmStatic external fun nativeUndoDo(): String
    @JvmStatic external fun nativeUndoRedo(): String
    @JvmStatic external fun nativeUndoGetState(): String

    // --- Room Permissions ---

    @JvmStatic external fun nativePermParse(powerLevelsJson: String): String
    @JvmStatic external fun nativePermGetRole(userId: String, powerLevel: Int): String
    @JvmStatic external fun nativePermBuildContent(powerLevelsJson: String): String
    @JvmStatic external fun nativePermBuildKick(userId: String, reason: String): String
    @JvmStatic external fun nativePermBuildBan(userId: String, reason: String): String
    @JvmStatic external fun nativePermFormatChange(userId: String, oldPower: Int, newPower: Int): String

    // --- Offline Cache Manager ---

    @JvmStatic external fun nativeCacheRegisterRoom(roomJson: String)
    @JvmStatic external fun nativeCacheGetPlan(): String
    @JvmStatic external fun nativeCacheGetStats(): String
    @JvmStatic external fun nativeCacheGetPressure(availableBytes: Long, reservedBytes: Long): String
    @JvmStatic external fun nativeCacheEvictToFree(targetBytes: Long, availableBytes: Long, reservedBytes: Long): String
    @JvmStatic external fun nativeCacheRecordHit(roomId: String, bytes: Long)
    @JvmStatic external fun nativeCacheRecordMiss(roomId: String, bytes: Long)

    // --- Spoiler Manager ---

    @JvmStatic external fun nativeSpoilerBuildImage(body: String, mxcUrl: String, mimeType: String, width: Int, height: Int, sizeBytes: Long, reason: String): String
    @JvmStatic external fun nativeSpoilerBuildText(body: String, reason: String): String
    @JvmStatic external fun nativeSpoilerHasSpoiler(formattedBody: String): Boolean
    @JvmStatic external fun nativeSpoilerDetectType(formattedBody: String): String
    @JvmStatic external fun nativeSpoilerBuildContent(body: String, mxcUrl: String, msgType: String, reason: String): String

    // --- WebRTC Utils ---

    @JvmStatic external fun nativeFormatCallDuration(seconds: Int): String

    // --- Message Retry ---


    // --- Sync Utils ---
    // --- Event Display ---


    // --- Permalink ---

    @JvmStatic external fun nativeBuildEventPermalink(roomId: String, eventId: String): String

    // --- Network Monitor ---


    // --- Client Info ---

    @JvmStatic external fun nativeCompareSemver(a: String, b: String): Int

    // --- Keyshare ---


    // --- Display Name ---

    @JvmStatic external fun nativeUserIdToDisplayName(userId: String): String

    // --- Message Location ---


    // --- Timeline Utils ---


    // --- Cross Signing ---
    // --- Edit History ---

    @JvmStatic external fun nativeGetEditBadgeText(editCount: Int): String

    // --- Read Marker / Unread Count ---
    // Ported from: TimelineViewModel.kt (read marker index math)
    //              ReadMarkers.kt (server-side read marker management)
    //              RoomSummary.kt (unread count display)

    /**
     * Compute the read marker position and unread statistics.
     * @return JSON with lastReadEventId, firstUnreadEventId, unreadCount,
     *         unreadMentions, hasUnread, readMarkerIndex
     */
        lastReadEventId: String,
        loadedEventIds: Array<String>,
        loadedSenders: Array<String>,
        isMention: BooleanArray,
        isHighlight: BooleanArray,
        myUserId: String
    ): String

    @JvmStatic external fun nativeAdvanceReadMarker(roomId: String, latestEventId: String): String
    @JvmStatic external fun nativeReadMarkerToJson(lastReadEventId: String, unreadCount: Int, unreadMentions: Int, unreadHighlights: Int, hasUnread: Boolean): String

    // ---- IDN (Internationalized Domain Names) ----
    @JvmStatic external fun nativeToPunycode(domain: String): String
    @JvmStatic external fun nativeFromPunycode(domain: String): String

    @JvmStatic external fun nativeFormatCountToShortDecimal(value: Int): String

}
