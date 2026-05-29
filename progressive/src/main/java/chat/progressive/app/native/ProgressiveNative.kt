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
    external fun nativeValidateAndBuild(
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
    external fun nativeParseResponse(
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
    external fun nativeFormatEventHtml(
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
    external fun nativeFormatEventPlainText(
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        fileName: String,
        relationType: String
    ): String

    @JvmStatic
    external fun nativeBuildHtmlExport(
        roomName: String,
        roomTopic: String,
        exportDate: String,
        eventHtmls: Array<String>
    ): String

    // --- Event Cache ---

    @JvmStatic
    external fun nativeCachePut(
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
    external fun nativeCacheGetContext(eventId: String): String

    @JvmStatic
    external fun nativeCacheClear()

    @JvmStatic
    external fun nativeCacheSize(): Int

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
    external fun nativeComputeProxyConfig(
        connType: Int,
        proxyType: Int,
        host: String,
        port: Int,
        username: String,
        password: String
    ): String

    // --- Yggdrasil ---

    @JvmStatic external fun nativeIsYggdrasilAddress(addr: String): Boolean
    @JvmStatic external fun nativeIsYggdrasilDomain(host: String): Boolean
    @JvmStatic external fun nativeBuildYggHomeserverUrl(addr: String, port: Int, tls: Boolean): String
    @JvmStatic external fun nativeRewriteHomeserverUrl(originalUrl: String, yggAddr: String): String

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

    @JvmStatic external fun nativeIsSupportedAudioType(mimeType: String): Boolean
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

    @JvmStatic external fun nativeEncryptAccount(
        userId: String, token: String, refreshToken: String,
        homeServer: String, deviceId: String, deviceName: String,
        displayName: String, avatarUrl: String,
        includeCache: Boolean, passphrase: String
    ): String

    @JvmStatic external fun nativeDecryptAccount(encrypted: String, passphrase: String): String

    // --- Audio ---

    @JvmStatic external fun nativeFormatDuration(ms: Long): String
    @JvmStatic external fun nativeComputeProgress(posMs: Long, durMs: Long): Float
    @JvmStatic external fun nativeIsSupportedAudio(mimeType: String): Boolean

    // --- Media Filter ---

    @JvmStatic external fun nativeGetFileExtension(fileName: String, mimeType: String): String
    @JvmStatic external fun nativeIsValidMxcUri(uri: String): Boolean

    // --- Content Filter ---

    @JvmStatic external fun nativeKeywordFilterLoad(raw: String)
    @JvmStatic external fun nativeKeywordFilterCheck(text: String): String
    @JvmStatic external fun nativeKeywordFilterExport(): String
    @JvmStatic external fun nativeKeywordFilterCount(): Int
    @JvmStatic external fun nativeKeywordFilterClear()

    @JvmStatic external fun nativeShouldBlockImage(
        blockAll: Boolean, allowAvatars: Boolean, allowStickers: Boolean, allowEmoji: Boolean,
        mxcUrl: String, imageType: String
    ): Boolean

    // --- Network Stats ---

    @JvmStatic external fun nativeNetStatsStart(url: String, method: String): Int
    @JvmStatic external fun nativeNetStatsEnd(requestId: Int, statusCode: Int, bytesSent: Long, bytesReceived: Long, error: String)
    @JvmStatic external fun nativeNetStatsToJson(): String
    @JvmStatic external fun nativeNetStatsToText(): String
    @JvmStatic external fun nativeNetStatsClear()

    // --- Masquerade ---

    @JvmStatic external fun nativeIsValidMasqueradeName(name: String): Boolean
    @JvmStatic external fun nativeGetSuggestedMasqueradeNames(): String
    @JvmStatic external fun nativeIsValidIconAlias(alias: String): Boolean
    @JvmStatic external fun nativeBuildMasqueradeAlias(baseAlias: String, iconName: String): String

    // --- User Mask ---

    @JvmStatic external fun nativeUserMaskSet(mxid: String, displayName: String, avatarUrl: String, overrideMxid: String)
    @JvmStatic external fun nativeUserMaskRemove(mxid: String)
    @JvmStatic external fun nativeUserMaskResolveName(mxid: String, originalName: String): String
    @JvmStatic external fun nativeUserMaskResolveAvatar(mxid: String, originalUrl: String): String
    @JvmStatic external fun nativeUserMaskExportJson(): String
    @JvmStatic external fun nativeUserMaskImportJson(json: String)
    @JvmStatic external fun nativeIsValidMxid(mxid: String): Boolean
    @JvmStatic external fun nativeUserMaskClear()
    @JvmStatic external fun nativeUserMaskCount(): Int

    // --- Chunked Upload ---

    @JvmStatic external fun nativeUploaderGetChunkJson(index: Int): String
    @JvmStatic external fun nativeUploaderFail(error: String)
    @JvmStatic external fun nativeUploaderProgressJson(): String

    // --- Chat Features (Timezone + EXIF) ---

    @JvmStatic external fun nativeGetCommonTimezones(): String
    @JvmStatic external fun nativeFormatTimestampInTimezone(utcMs: Long, tzId: String): String
    @JvmStatic external fun nativeIsValidTimezoneId(tzId: String): Boolean
    @JvmStatic external fun nativeFileHasMetadata(mimeType: String): Boolean
    @JvmStatic external fun nativeGetStrippableMimeTypes(): String

    // --- Invitation Hide ---

    @JvmStatic external fun nativeInviteHide(roomId: String, roomName: String, inviterName: String, inviterMxid: String)
    @JvmStatic external fun nativeInviteUnhide(roomId: String)
    @JvmStatic external fun nativeInviteIsHidden(roomId: String): Boolean
    @JvmStatic external fun nativeInviteExportJson(): String
    @JvmStatic external fun nativeInviteImportJson(json: String)
    @JvmStatic external fun nativeInviteClear()
    @JvmStatic external fun nativeInviteCount(): Int

    // --- Thread Aggregator ---

    @JvmStatic external fun nativeThreadAdd(
        threadId: String, roomId: String, roomName: String,
        accountId: String, accountIndex: String,
        lastMsg: String, lastSender: String,
        lastTs: Long, replyCount: Int, unread: Boolean
    )
    @JvmStatic external fun nativeThreadGetAllJson(): String
    @JvmStatic external fun nativeThreadClear()
    @JvmStatic external fun nativeThreadCount(): Int
    @JvmStatic external fun nativeThreadRemoveRoom(roomId: String)

    // --- User Messages ---

    @JvmStatic external fun nativeFormatUserMessagePreview(roomName: String, body: String, msgType: String, maxLen: Int): String

    // --- Room Version ---

    @JvmStatic external fun nativeGetRoomVersionsJson(): String
    @JvmStatic external fun nativeIsValidRoomVersion(version: String): Boolean

    // --- Chat Preview ---

    @JvmStatic external fun nativeFormatShortTime(epochMs: Long): String
    @JvmStatic external fun nativeTruncateMessage(body: String, maxLen: Int): String

    // --- RAM Monitor ---

    @JvmStatic external fun nativeGetMemoryInfo(): String
    @JvmStatic external fun nativeFormatMemoryLabel(rssKb: Long): String

    // --- Cache Manager ---

    @JvmStatic external fun nativeCacheTrack(eventId: String, roomId: String, roomName: String, timestamp: Long, sizeBytes: Long, msgType: String, body: String)
    @JvmStatic external fun nativeCacheStatsJson(): String
    @JvmStatic external fun nativeCacheGetByRoom(roomId: String): String
    @JvmStatic external fun nativeCacheGetOlderThan(beforeTs: Long): String
    // --- Message Aggregator (All Messages) ---

    @JvmStatic external fun nativeMsgAggAdd(eventId: String, roomId: String, roomName: String, accountId: String, accountIndex: String, senderName: String, body: String, msgType: String, originServerTs: Long)
    @JvmStatic external fun nativeMsgAggGetAllJson(): String
    @JvmStatic external fun nativeMsgAggClear()
    @JvmStatic external fun nativeMsgAggCount(): Int

    // --- Room Info ---

    @JvmStatic external fun nativeFormatCreationDate(epochMs: Long): String
    @JvmStatic external fun nativeIsLikelyFullHistory(cached: Int, estimated: Int): Boolean

    // --- Deleted Archive ---

    @JvmStatic external fun nativeDeletedArchiveAdd(eventId: String, roomId: String, roomName: String, senderName: String, body: String, msgType: String, timestamp: String, originTs: Long, deletedBy: String)
    @JvmStatic external fun nativeDeletedArchiveExportJson(): String
    @JvmStatic external fun nativeDeletedArchiveClear()
    @JvmStatic external fun nativeDeletedArchiveCount(): Int

    // --- Search Index ---

    @JvmStatic external fun nativeSearchIndexMessage(eventId: String, roomId: String, roomName: String, senderName: String, body: String, timestamp: Long, isEncrypted: Boolean)
    @JvmStatic external fun nativeSearchQuery(query: String, limit: Int): String
    @JvmStatic external fun nativeSearchClear()
    @JvmStatic external fun nativeSearchIndexedCount(): Int

    // --- Module Loader ---

    @JvmStatic external fun nativeModuleScanDir(dirPath: String)
    @JvmStatic external fun nativeModuleEnable(name: String)
    @JvmStatic external fun nativeModuleIsEnabled(name: String): Boolean
    @JvmStatic external fun nativeModuleListJson(): String

    // --- Notification Keywords ---

    @JvmStatic external fun nativeNotifKeywordAdd(keyword: String, caseSensitive: Boolean)
    @JvmStatic external fun nativeNotifKeywordCheck(body: String): String
    @JvmStatic external fun nativeNotifKeywordExport(): String
    @JvmStatic external fun nativeNotifKeywordImport(json: String)
    @JvmStatic external fun nativeNotifKeywordClear()

    // --- Reaction Preview ---

    @JvmStatic external fun nativeFormatReactionPreview(reactorName: String, emoji: String, sourceBody: String, sourceSender: String): String

    // --- Room Mirror ---

    @JvmStatic external fun nativeMirrorAdd(srcRoomId: String, srcRoomName: String, mirRoomId: String, mirRoomName: String, enabled: Boolean, useDolls: Boolean)
    @JvmStatic external fun nativeMirrorRemove(srcRoomId: String)
    @JvmStatic external fun nativeMirrorIsActive(srcRoomId: String): Boolean
    @JvmStatic external fun nativeMirrorFormatMessage(senderName: String, senderMxid: String, roomName: String, body: String, msgType: String, ts: Long): String
    @JvmStatic external fun nativeMirrorGenerateDollMxid(originalMxid: String, targetServer: String): String
    @JvmStatic external fun nativeMirrorIsValidDoll(mxid: String): Boolean
    @JvmStatic external fun nativeMirrorExportJson(): String

    // --- Input Tools ---

    @JvmStatic external fun nativeSymbolAdd(symbol: String, label: String)
    @JvmStatic external fun nativeSymbolExport(): String
    @JvmStatic external fun nativeSymbolImport(json: String)

    @JvmStatic external fun nativeReplacementAddRule(pattern: String, replacement: String, exactMatch: Boolean)
    @JvmStatic external fun nativeReplacementApply(text: String): String
    @JvmStatic external fun nativeReplacementExport(): String

    // --- LLM ---

    @JvmStatic external fun nativeBuildLlmRequest(prompt: String, provider: Int, endpoint: String, token: String, model: String, systemPrompt: String, temp: Float, maxTokens: Int): String
    @JvmStatic external fun nativeBuildLlmHeaders(provider: Int, token: String): String
    @JvmStatic external fun nativeParseLlmResponse(body: String, statusCode: Int, provider: Int): String
    @JvmStatic external fun nativeFormatLlmBroadcast(prompt: String, response: String): String

    // --- Room Summary ---

    @JvmStatic external fun nativeAlarmGetWeatherAction(alarmId: String): String

    @JvmStatic external fun nativeFormatRoomSummary(roomId: String, name: String, body: String, sender: String, ts: Long, notif: Int, hl: Int, direct: Boolean): String
    @JvmStatic external fun nativeFormatNotifBadge(count: Int, highlight: Int): String

    // --- Smart Reply ---

    @JvmStatic external fun nativeSmartReplyPrompt(lastMsg: String, sender: String, room: String, isDirect: Boolean): String
    @JvmStatic external fun nativeSmartParseReplies(llmResponse: String): String
    @JvmStatic external fun nativeSmartDefaultReplies(lastMsg: String): String
    @JvmStatic external fun nativeSmartDefaultReactions(): String

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

    @JvmStatic external fun nativeParseGeoUri(uri: String): String
    @JvmStatic external fun nativeBuildLocationContent(lat: Double, lon: Double, desc: String): String
    @JvmStatic external fun nativeBuildStaticMap(lat: Double, lon: Double, zoom: Int, width: Int, height: Int): String
    @JvmStatic external fun nativeHaversine(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double

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

    @JvmStatic external fun nativeFormatUserDisplayName(displayName: String, mxid: String, showMxid: Boolean): String

    // --- MXID Visibility ---

    @JvmStatic external fun nativeMxidVisibilityHide(mxid: String)
    @JvmStatic external fun nativeMxidVisibilityShow(mxid: String)
    @JvmStatic external fun nativeMxidVisibilityIsVisible(mxid: String): Boolean
    @JvmStatic external fun nativeMxidVisibilityExport(): String

    // --- Read Receipts ---

    @JvmStatic external fun nativeComputeReceiptDisplay(receiptsJson: String, maxVisible: Int): String

    // --- Room Analytics ---

    @JvmStatic external fun nativeExtractServerName(mxid: String): String

    // --- User Hide Timer ---

    @JvmStatic external fun nativeUserHideFor(userId: String, displayName: String, minutes: Int)
    @JvmStatic external fun nativeUserHideIsHidden(userId: String): Boolean
    @JvmStatic external fun nativeUserHideGetActive(): String

    // --- Message Queue ---

    @JvmStatic external fun nativeMsgQueueEnqueue(msgId: String, roomId: String, body: String, formattedBody: String, order: Int, maxRetries: Int)
    @JvmStatic external fun nativeMsgQueueSetOrder(msgId: String, order: Int)
    @JvmStatic external fun nativeMsgQueueMarkFailed(msgId: String, error: String)
    @JvmStatic external fun nativeMsgQueueMarkSent(msgId: String)
    @JvmStatic external fun nativeMsgQueuePendingCount(): Int
    @JvmStatic external fun nativeMsgQueueExport(): String

    // --- Image Crop ---

    @JvmStatic external fun nativeIsValidCrop(imgW: Int, imgH: Int, x: Int, y: Int, w: Int, h: Int): Boolean

    // --- Auto-Scroll ---

    @JvmStatic external fun nativeComputeScrollPlan(smooth: Boolean, durationMin: Int, totalLines: Int, lineHeightPx: Int): String

    // --- Language Detection ---

    @JvmStatic external fun nativeDetectLanguage(text: String, method: Int): String
    @JvmStatic external fun nativeGetLanguageLabel(code: String): String

    // --- Language Hide ---

    @JvmStatic external fun nativeLangHideAdd(langCode: String, roomId: String, userId: String, specificUser: Boolean, minutes: Int)
    @JvmStatic external fun nativeLangHideIsHidden(langCode: String, roomId: String, userId: String): Boolean

    // --- Chat Push Down ---

    @JvmStatic external fun nativeChatPushDown(roomId: String, minutes: Int)
    @JvmStatic external fun nativeChatIsPushedDown(roomId: String): Boolean
    @JvmStatic external fun nativeChatPushDownRestore(roomId: String)

    // --- Emoji Blacklist ---

    @JvmStatic external fun nativeEmojiBlacklistAdd(emoji: String)
    @JvmStatic external fun nativeEmojiBlacklistRemove(emoji: String)
    @JvmStatic external fun nativeEmojiBlacklistIsBlocked(emoji: String): Boolean
    @JvmStatic external fun nativeEmojiBlacklistExport(): String
    @JvmStatic external fun nativeEmojiBlacklistImport(json: String)

    // --- Avatar History ---

    @JvmStatic external fun nativeAvatarAddChange(mxcUrl: String, eventId: String, timestamp: Long)
    @JvmStatic external fun nativeAvatarExportJson(): String
    @JvmStatic external fun nativeAvatarClear()

    // --- Jump to Date with Time ---

    @JvmStatic external fun nativeParseJumpToDate(input: String): String

    // --- Room Matching ---

    @JvmStatic external fun nativeMatchRooms(query: String, roomsJson: String): String
    @JvmStatic external fun nativeIsRoomId(input: String): Boolean
    @JvmStatic external fun nativeIsRoomAlias(input: String): Boolean

    // --- Event Links ---

    @JvmStatic external fun nativeExtractEventLinks(body: String): String
    @JvmStatic external fun nativeFormatResolvedEvent(sender: String, body: String): String
    @JvmStatic external fun nativeIsEventId(text: String): Boolean

    // --- Timestamps ---

    @JvmStatic external fun nativeFormatTimestamp(epochMs: Long, includeSeconds: Boolean): String
    @JvmStatic external fun nativeFormatFullTimestamp(epochMs: Long): String

    // --- Lightweight Call ---

    @JvmStatic external fun nativeLightCallEnter(): String
    @JvmStatic external fun nativeLightCallExit(): String
    @JvmStatic external fun nativeLightCallAssessMemory(): String
    @JvmStatic external fun nativeShouldUseLightweightMode(): Boolean

    // --- Scheduled Edits ---

    @JvmStatic external fun nativeSchedEditSchedule(roomId: String, targetEventId: String, newContent: String, contentUrl: String, formattedContent: String, formattedUrl: String, scheduledAtMs: Long, recurring: Boolean): String
    @JvmStatic external fun nativeSchedEditCancel(editId: String)
    @JvmStatic external fun nativeSchedEditGetDue(): String
    @JvmStatic external fun nativeSchedEditMarkApplied(editId: String)
    @JvmStatic external fun nativeSchedEditMarkFailed(editId: String, error: String)
    @JvmStatic external fun nativeSchedEditExport(): String
    @JvmStatic external fun nativeSchedEditStats(): String

    // --- SVG Rendering ---

    @JvmStatic external fun nativeParseSvg(svgData: String): String
    @JvmStatic external fun nativeIsValidSvg(data: String): Boolean

    // --- Drawing Canvas ---

    @JvmStatic external fun nativeDrawMoveTo(x: Double, y: Double)
    @JvmStatic external fun nativeDrawLineTo(x: Double, y: Double)
    @JvmStatic external fun nativeDrawSetColor(argb: Int)
    @JvmStatic external fun nativeDrawSetWidth(w: Double)
    @JvmStatic external fun nativeDrawExportJson(): String
    @JvmStatic external fun nativeDrawToSvgPath(): String
    @JvmStatic external fun nativeDrawClear()

    // --- Profile Swiper ---

    @JvmStatic external fun nativeProfileSwiperSetProfiles(profilesJson: String)
    @JvmStatic external fun nativeProfileSwiperNext(): String
    @JvmStatic external fun nativeProfileSwiperPrev(): String

    // --- Rainbow Generator ---

    @JvmStatic external fun nativeGenerateRainbow(text: String): String

    // --- Text Formatting ---

    @JvmStatic external fun nativeFormatSpoiler(text: String): String
    @JvmStatic external fun nativeFormatEmote(sender: String, text: String): String
    @JvmStatic external fun nativeFormatShrug(text: String): String
    @JvmStatic external fun nativeFormatLenny(text: String): String
    @JvmStatic external fun nativeFormatTableFlip(text: String): String
    @JvmStatic external fun nativeFormatPlain(text: String): String
    @JvmStatic external fun nativeIsEmojiOnly(text: String): Boolean
    @JvmStatic external fun nativeTruncateText(text: String, maxLen: Int): String

    // --- URL Tools ---

    @JvmStatic external fun nativeExtractFirstUrl(text: String): String
    @JvmStatic external fun nativeUrlEncode(input: String): String
    @JvmStatic external fun nativeUrlDecode(input: String): String
    @JvmStatic external fun nativeBuildMatrixToUrl(roomId: String): String

    // --- Notification Priority ---

    @JvmStatic external fun nativeComputeNotifPriority(isDM: Boolean, isMention: Boolean, isRoomMention: Boolean, isKeyword: Boolean, isCall: Boolean, isBackground: Boolean, dnd: Boolean, favorite: Boolean): String
    @JvmStatic external fun nativeFormatNotifTitle(roomName: String, senderName: String, isDM: Boolean): String
    @JvmStatic external fun nativeFormatNotifBody(body: String, senderName: String, isDM: Boolean, showSender: Boolean): String
    @JvmStatic external fun nativeIsRoomMention(body: String): Boolean

    // --- Matrix Patterns ---

    @JvmStatic external fun nativeIsUserId(input: String): Boolean
    @JvmStatic external fun nativeParseMatrixToPermalink(url: String): String
    @JvmStatic external fun nativeIsValidEmail(input: String): Boolean

    // --- Desync Detector ---

    @JvmStatic external fun nativeDesyncTrackEvent(eventId: String, serverName: String, timestamp: Long)
    @JvmStatic external fun nativeDesyncCheck(roomId: String, currentServer: String): String

    // --- Latency Tracker ---

    @JvmStatic external fun nativeLatencyRecord(latencyMs: Double, server: String, endpoint: String, success: Boolean)
    @JvmStatic external fun nativeLatencyStats(): String
    @JvmStatic external fun nativeLatencyStatsText(): String

    // --- String Utils ---

    @JvmStatic external fun nativeSanitizeRoomName(input: String): String
    @JvmStatic external fun nativeWordCount(input: String): Int

    // --- Location Sharing ---

    @JvmStatic external fun nativeLocationFormatMessage(lat: Double, lon: Double, acc: Double, label: String): String
    @JvmStatic external fun nativeLocationFormatGeoJson(lat: Double, lon: Double, acc: Double): String
    @JvmStatic external fun nativeLocationDistance(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double

    // --- Color Utils ---

    @JvmStatic external fun nativeContrastRatio(fgR: Int, fgG: Int, fgB: Int, bgR: Int, bgG: Int, bgB: Int): Double
    @JvmStatic external fun nativeParseColor(input: String): String

    // --- E2EE Utils ---

    @JvmStatic external fun nativeGetTrustLabel(level: Int): String
    @JvmStatic external fun nativeGetTrustBadge(level: Int): String

    // --- Thumbnail ---

    @JvmStatic external fun nativeComputeThumbnail(srcW: Int, srcH: Int, maxW: Int, maxH: Int, upscale: Boolean, quality: Int): String
    @JvmStatic external fun nativeBuildThumbnailUrl(mxcUri: String, w: Int, h: Int, method: String, animated: Boolean): String

    // --- Waveform ---

    @JvmStatic external fun nativeSuggestBarCount(durationMs: Long): Int
    @JvmStatic external fun nativeComputeRmsVolume(samples: IntArray): Double

    // --- Session Timeout ---

    @JvmStatic external fun nativeShouldLock(lockMethod: Int, idleTimeoutMin: Int, maxSessionMin: Int, maxFailedPin: Int, lockOnBg: Boolean, lastActivityMs: Long, sessionStartMs: Long, failedAttempts: Int, isLocked: Boolean, isBackground: Boolean): Boolean
    @JvmStatic external fun nativeIsValidPin(pin: String, minLen: Int, maxLen: Int): Boolean

    // --- Password Validator ---

    @JvmStatic external fun nativeValidatePassword(password: String): String

    // --- Spellcheck ---

    @JvmStatic external fun nativeEditDistance(a: String, b: String): Int

    // --- Typing Indicator ---

    @JvmStatic external fun nativeFormatTypingText(namesJson: String): String

    // --- Hash Utils ---

    @JvmStatic external fun nativeSha256Hex(input: String): String
    @JvmStatic external fun nativeGenerateToken(bytes: Int): String

    // --- Room Stats ---

    @JvmStatic external fun nativeComputeMessagesPerDay(count: Int, firstTs: Long, lastTs: Long): Double

    // --- Mention Parser ---

    @JvmStatic external fun nativeBuildUserPill(userId: String, displayName: String): String
    @JvmStatic external fun nativeStripPills(html: String): String

    // --- Poll Utils ---
    // --- Reaction Utils ---

    @JvmStatic external fun nativeGetQuickReactions(): String

    // --- File Validator ---

    @JvmStatic external fun nativeFormatFileSize(bytes: Long): String
    @JvmStatic external fun nativeGetExtensionFromName(fileName: String): String

    // --- Date Utils ---

    @JvmStatic external fun nativeFormatChatTimestamp(epochMs: Long, includeSeconds: Boolean): String
    @JvmStatic external fun nativeFormatRelativeTime(epochMs: Long): String

    // --- Message Queue ---

    @JvmStatic external fun nativeTextSimilarity(a: String, b: String): Double

    // --- Pinned Events (Element Web parity) ---

    @JvmStatic external fun nativeParsePinnedEventIds(stateJson: String): String
    @JvmStatic external fun nativeBuildPinnedEventsContent(idsJson: String): String

    // --- Server Capabilities ---

    @JvmStatic external fun nativeParseServerCapabilities(wellKnownJson: String): String

    // --- Username Validator ---

    @JvmStatic external fun nativeValidateUsername(username: String): String

    // --- Emoji Analyzer ---

    @JvmStatic external fun nativeIsEmojiOnlyMessage(text: String): Boolean

    // --- Identity Utils ---

    @JvmStatic external fun nativeResolveMatrixId(input: String): String
    @JvmStatic external fun nativeGetInitials(name: String): String

    // --- Notification Analyzer ---

    @JvmStatic external fun nativeSuggestQuietHours(byHourJson: String): String

    // --- Sync Analyzer ---

    @JvmStatic external fun nativeFormatProgressBar(percent: Double, width: Int): String

    // --- User Rating ---

    @JvmStatic external fun nativeComputeStreak(timestampsJson: String): String

    // --- Event Timeline ---

    @JvmStatic external fun nativeFormatGroupLabel(timestampMs: Long): String

    // --- Room Directory ---

    @JvmStatic external fun nativeExtractServers(roomsJson: String): String

    // --- SSO Utils ---

    @JvmStatic external fun nativeValidateHomeserverUrl(input: String): String

    // --- Backup Utils ---

    @JvmStatic external fun nativeIsValidRecoveryKey(key: String): Boolean

    // --- Device Manager ---

    @JvmStatic external fun nativeFormatDeviceLastSeen(lastSeenMs: Long): String
    @JvmStatic external fun nativeIsDeviceInactive(lastSeenMs: Long): Boolean

    // --- Presence ---

    @JvmStatic external fun nativeFormatPresence(presence: Int): String
    @JvmStatic external fun nativeFormatPresenceLine(presence: Int, lastActiveAgoMs: Long, statusMsg: String): String

    // --- Room Permissions ---

    @JvmStatic external fun nativeGetSuggestedRole(plJson: String, userId: String): String

    // --- Room Summary ---

    @JvmStatic external fun nativeFormatLastMessagePreview(sender: String, body: String, encrypted: Boolean): String

    // --- Membership ---

    @JvmStatic external fun nativeFormatMembership(membershipStr: String): String

    // --- Event Validator ---

    @JvmStatic external fun nativeValidateEvent(eventId: String, eventType: String, senderId: String, contentJson: String, originTs: String, blockedUsersJson: String): String

    // --- Room Encryption ---

    @JvmStatic external fun nativeIsRoomEncrypted(stateJson: String): Boolean

    // --- Login Utils ---

    @JvmStatic external fun nativeBuildLoginBody(userId: String, password: String, deviceName: String, deviceId: String): String

    // --- Account Utils ---

    @JvmStatic external fun nativeValidatePasswordWithUsername(password: String, username: String): String

    // --- Connection Monitor ---

    @JvmStatic external fun nativeConnMonitorOnConnected()
    @JvmStatic external fun nativeConnMonitorOnDisconnected()
    @JvmStatic external fun nativeConnMonitorGetStatus(): String

    // --- Push Rules ---

    @JvmStatic external fun nativeParsePushCondition(conditionJson: String): String

    // --- Space Utils ---

    @JvmStatic external fun nativeBuildSpaceChildContent(suggested: Boolean, order: String, autoJoin: Boolean, canonical: Boolean): String

    // --- Event Relations ---
    // --- E2EE Decoration ---

    @JvmStatic external fun nativeComputeE2eeDecoration(encrypted: Boolean, verified: Boolean, crossSigned: Boolean, decryptError: Boolean, blacklisted: Boolean, beforeJoined: Boolean, errorReason: String): String

    // --- Room List ---

    @JvmStatic external fun nativeGetBadgeText(count: Int, highlights: Int): String

    // --- Media Utils ---

    @JvmStatic external fun nativeMimeToMsgType(mimeType: String): String
    @JvmStatic external fun nativeIsValidBlurhash(hash: String): Boolean

    // --- Notification Settings ---

    @JvmStatic external fun nativeFormatNotifMode(mode: Int): String

    // --- Invite Utils ---

    @JvmStatic external fun nativeBuildInviteBody(userId: String, reason: String): String

    // --- Verification ---

    @JvmStatic external fun nativeGetVerificationEmojis(): String

    // --- Session Manager ---

    @JvmStatic external fun nativeFormatSessionBadge(unread: Int, highlights: Int): String

    // --- Auth Utils ---

    @JvmStatic external fun nativeFormatRateLimitMessage(responseJson: String, httpStatus: Int): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeIsServerNotice(contentJson: String): Boolean

    // --- Event Encryption ---

    @JvmStatic external fun nativeParseEncryptedHeader(contentJson: String): String

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
    @JvmStatic external fun nativeExtractServerNameFromId(mxid: String): String
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
    @JvmStatic external fun nativeFormatSpoilerTextFromHtml(formattedBody: String): String
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
    @JvmStatic external fun nativeThreadAddReply(threadId: String, senderId: String, senderName: String, body: String, timestampMs: Long)
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
    @JvmStatic external fun nativeIsCallEvent(eventType: String): Boolean

    // --- Message Retry ---

    @JvmStatic external fun nativeComputeRetryDelay(retryCount: Int, maxDelayMs: Int): Int
    @JvmStatic external fun nativeDecideRetry(retryCount: Int, errorCode: Int, retryAfterHeader: String): String
    @JvmStatic external fun nativeFormatMessageStatus(state: Int): String

    // --- Sync Utils ---
    // --- Event Display ---

    @JvmStatic external fun nativeClassifyEvent(eventType: String, msgType: String): Int

    // --- Permalink ---

    @JvmStatic external fun nativeBuildEventPermalink(roomId: String, eventId: String): String

    // --- Network Monitor ---

    @JvmStatic external fun nativeGetRecommendedMediaQuality(type: Int, connected: Boolean, metered: Boolean, signal: Int, latency: Double, loss: Double): String

    // --- Client Info ---

    @JvmStatic external fun nativeCompareSemver(a: String, b: String): Int

    // --- Keyshare ---

    @JvmStatic external fun nativeBuildKeyRequestBody(roomId: String, sessionId: String, senderKey: String, algorithm: String, requestId: String, deviceId: String): String

    // --- Display Name ---

    @JvmStatic external fun nativeUserIdToDisplayName(userId: String): String
    @JvmStatic external fun nativeUserIdToColor(userId: String): String

    // --- Message Location ---

    @JvmStatic external fun nativeEstimatePaginationRequests(missingEvents: Int, pageSize: Int): Int

    // --- Timeline Utils ---

    @JvmStatic external fun nativeShouldAutoScroll(isOwnMessage: Boolean): Boolean

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
    @JvmStatic external fun nativeComputeReadMarker(
        lastReadEventId: String,
        loadedEventIds: Array<String>,
        loadedSenders: Array<String>,
        isMention: BooleanArray,
        isHighlight: BooleanArray,
        myUserId: String
    ): String

    @JvmStatic external fun nativeAdvanceReadMarker(roomId: String, latestEventId: String): String
    @JvmStatic external fun nativeReadMarkerToJson(lastReadEventId: String, unreadCount: Int, unreadMentions: Int, unreadHighlights: Int, hasUnread: Boolean): String

    // --- Kotlin fallbacks (765 minimal stubs) ---

    @JvmStatic fun addBreadcrumbFallback(currentJson: String, roomId: String): String { return "" }

    @JvmStatic fun advanceReadMarkerFallback(roomId: String, latestEventId: String): String { return "" }

    @JvmStatic fun assignJoinOrderFallback(roomsJson: String, accountCount: Int): String { return "" }

    @JvmStatic fun buildDraftMessageFallback(prefix: String, text: String): String { return "" }

    @JvmStatic fun buildEditRelationFallback(eventId: String): String { return "" }

    @JvmStatic fun buildFileContentFallback(body: String, mxcUrl: String, fileName: String, sz: Long, mime: String): String { return "" }

    @JvmStatic fun buildHtmlExportFallback(roomName: String, roomTopic: String, exportDate: String, eventHtmls: Array<String>): String { return "" }

    @JvmStatic fun buildImageContentFallback(body: String, mxcUrl: String, w: Int, h: Int, sz: Long, mime: String): String { return "" }

    @JvmStatic fun buildOAuthUrlFallback(clientId: String, redirectUri: String, state: String, codeChallenge: String, prompt: String): String { return "" }

    @JvmStatic fun buildReactionRelationFallback(eventId: String, key: String): String { return "" }

    @JvmStatic fun buildReplyRelationFallback(eventId: String): String { return "" }

    @JvmStatic fun buildRoomStateContentFallback(eventType: String, value1: String, value2: String): String { return "" }

    @JvmStatic fun buildSearchUrlFallback(engine: String, endpoint: String, apiKey: String, engineId: String, query: String, maxResults: Int): String { return "" }

    @JvmStatic fun buildTextContentFallback(msgType: String, body: String, formattedBody: String): String { return "" }

    @JvmStatic fun buildThreadRelationFallback(rootId: String, latestId: String, fallingBack: Boolean): String { return "" }

    @JvmStatic fun buildTranslateRequestFallback(text: String, sourceLang: String, targetLang: String, apiEndpoint: String, apiToken: String, model: String): JSONObject { return JSONObject() }

    @JvmStatic fun cacheClearFallback(): Int { return 0 }

    @JvmStatic fun cacheGetContextFallback(eventId: String): JSONObject { return JSONObject() }

    @JvmStatic fun cachePutFallback(): String { return "" }

    @JvmStatic fun cacheSizeFallback(): Int { return 0 }

    @JvmStatic fun calculateCapabilitiesFallback(userLevel: Int, eventsDefault: Int, stateDefault: Int, inviteLvl: Int, kickLvl: Int, banLvl: Int, redactLvl: Int, notifyLvl: Int): String { return "" }

    @JvmStatic fun calculateThumbnailSizeFallback(origW: Int, origH: Int, maxW: Int, maxH: Int): String { return "" }

    @JvmStatic fun computeReadMarkerFallback(lastReadEventId: String, loadedEventIds: Array<String>, loadedSenders: Array<String>, isMention: BooleanArray, isHighlight: BooleanArray, myUserId: String): JSONObject { return JSONObject() }

    @JvmStatic fun countRoomsFallback(roomsJson: String, accountCount: Int, uniqueOnly: Boolean, perAccountSplit: Boolean): String { return "" }

    @JvmStatic fun discoverOidcFallback(homeserverUrl: String): String { return "" }

    @JvmStatic fun exchangeOidcCodeFallback(tokenEndpoint: String, clientId: String, redirectUri: String, code: String, codeVerifier: String): String { return "" }

    @JvmStatic fun finalizeDraftFallback(full: String, prefix: String): String { return "" }

    @JvmStatic fun formatDurationFallback(ms: Long): String { return "" }

    @JvmStatic fun formatEventHtmlFallback(senderName: String, timestamp: String, body: String, msgType: String, fileName: String, mediaSize: String, relationType: String, isContinuation: Boolean): String { return "" }

    @JvmStatic fun formatEventPlainTextFallback(senderName: String, timestamp: String, body: String, msgType: String, fileName: String, relationType: String): String { return "" }

    @JvmStatic fun formatEventSummaryFallback(eventType: String, msgType: String, senderName: String, body: String, membership: String, displayName: String, isRedacted: Boolean, isEncrypted: Boolean): String { return "" }

    @JvmStatic fun formatFileSizeFallback(bytes: Long): String { return "" }

    @JvmStatic fun formatSearchForAgentFallback(responseJson: String): String { return "" }

    @JvmStatic fun formatSlashCommandFallback(command: String, arguments: String, type: Int, sender: String): String { return "" }

    @JvmStatic fun formatTypingIndicatorFallback(namesJson: String, maxNames: Int): String { return "" }

    @JvmStatic fun formatUnreadJumpLabelFallback(unreadCount: Int, unreadMentions: Int): String { return "" }

    @JvmStatic fun generateOAuthStateFallback(): String { return "" }

    @JvmStatic fun generatePkceFallback(): String { return "" }

    @JvmStatic fun isDumpBetterFallback(candidateEventCount: Int, candidateStartMs: Long, candidateEndMs: Long, baselineEventCount: Int, baselineStartMs: Long, baselineEndMs: Long, candidateHasGaps: Boolean, baselineHasGaps: Boolean): Boolean { return false }

    @JvmStatic fun isValidUserIdFallback(userId: String): Boolean { return false }

    @JvmStatic fun nativeAcceptTermsBodyToJsonFallback(bodyJson: String): String { return "" }

    @JvmStatic fun nativeAgentExtractTextAnswerFallback(llmResponse: String): String { return "" }

    @JvmStatic fun nativeAgentGetToolsSchemaFallback(): String { return "" }

    @JvmStatic fun nativeAgentHasToolCallsFallback(llmResponse: String): Boolean { return false }

    @JvmStatic fun nativeAnnotateEditedFallback(body: String, isEdited: Boolean): String { return "" }

    @JvmStatic fun nativeApiAvailableFallback(): Boolean { return false }

    @JvmStatic fun nativeApiBanUserFallback(roomId: String, userId: String, reason: String): String { return "" }

    @JvmStatic fun nativeApiCreateFilterFallback(userId: String, filterJson: String): String { return "" }

    @JvmStatic fun nativeApiCreateRoomFallback(name: String, topic: String, isDirect: Boolean, invitees: String): String { return "" }

    @JvmStatic fun nativeApiGetDisplayNameFallback(userId: String): String { return "" }

    @JvmStatic fun nativeApiGetProfileFallback(userId: String): String { return "" }

    @JvmStatic fun nativeApiGetPushRulesFallback(): String { return "" }

    @JvmStatic fun nativeApiGetRoomMembersFallback(roomId: String): String { return "" }

    @JvmStatic fun nativeApiGetRoomMessagesFallback(roomId: String, from: String, dir: String, limit: Int): String { return "" }

    @JvmStatic fun nativeApiGetVersionsFallback(): String { return "" }

    @JvmStatic fun nativeApiInviteUserFallback(roomId: String, userId: String, reason: String): String { return "" }

    @JvmStatic fun nativeApiJoinRoomFallback(roomId: String, reason: String): String { return "" }

    @JvmStatic fun nativeApiKickUserFallback(roomId: String, userId: String, reason: String): String { return "" }

    @JvmStatic fun nativeApiLeaveRoomFallback(roomId: String): String { return "" }

    @JvmStatic fun nativeApiLoginFallback(userId: String, password: String, deviceId: String): String { return "" }

    @JvmStatic fun nativeApiLogoutAllFallback(): Boolean { return false }

    @JvmStatic fun nativeApiLogoutFallback(): Boolean { return false }

    @JvmStatic fun nativeApiPublicRoomsFallback(server: String, query: String, limit: Int): String { return "" }

    @JvmStatic fun nativeApiRedactEventFallback(roomId: String, eventId: String, txnId: String): String { return "" }

    @JvmStatic fun nativeApiSearchFallback(query: String, roomId: String, limit: Int): String { return "" }

    @JvmStatic fun nativeApiSendEventFallback(roomId: String, eventType: String, txnId: String, contentJson: String): String { return "" }

    @JvmStatic fun nativeApiSetDisplayNameFallback(userId: String, displayName: String): String { return "" }

    @JvmStatic fun nativeApiSyncFallback(filter: String, since: String, timeout: Int): String { return "" }

    @JvmStatic fun nativeApiUnbanUserFallback(roomId: String, userId: String): String { return "" }

    @JvmStatic fun nativeApiWhoAmIFallback(): String { return "" }

    @JvmStatic fun nativeApplyWidgetUrlTemplateFallback(url: String, templateJson: String): String { return "" }

    @JvmStatic fun nativeAreGuestsAllowedFallback(stateContentJson: String): Boolean { return false }

    @JvmStatic fun nativeBackupAdvanceDecryptedFallback(): String { return "" }

    @JvmStatic fun nativeBackupAdvanceDownloadedFallback(): String { return "" }

    @JvmStatic fun nativeBackupAdvanceImportedFallback(): String { return "" }

    @JvmStatic fun nativeBackupAdvanceUploadedFallback(): String { return "" }

    @JvmStatic fun nativeBackupBuildCreateVersionFallback(configJson: String): String { return "" }

    @JvmStatic fun nativeBackupBuildDeleteFallback(version: String): String { return "" }

    @JvmStatic fun nativeBackupDecryptAllFallback(keysJson: String, authData: String, recoveryKey: String): String { return "" }

    @JvmStatic fun nativeBackupDecryptSessionFallback(sessionJson: String, backupKey: String, roomId: String): String { return "" }

    @JvmStatic fun nativeBackupEncryptSessionFallback(sessionJson: String, authData: String): String { return "" }

    @JvmStatic fun nativeBackupExportSessionFallback(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String, firstMessageIndex: Long, isForwarded: Boolean, forwardedCount: Long): String { return "" }

    @JvmStatic fun nativeBackupExtractPrivateKeyFallback(recoveryKey: String): String { return "" }

    @JvmStatic fun nativeBackupGenerateRecoveryKeyFallback(curve25519Key: String): String { return "" }

    @JvmStatic fun nativeBackupMarkCompleteFallback(): String { return "" }

    @JvmStatic fun nativeBackupParseKeysFallback(backupJson: String): String { return "" }

    @JvmStatic fun nativeBackupParseVersionFallback(json: String): String { return "" }

    @JvmStatic fun nativeBackupProgressFallback(): String { return "" }

    @JvmStatic fun nativeBackupProgressJsonFallback(): String { return "" }

    @JvmStatic fun nativeBackupResetFallback(): String { return "" }

    @JvmStatic fun nativeBackupSetTotalKeysFallback(): String { return "" }

    @JvmStatic fun nativeBackupVerifyIntegrityFallback(authData: String): Boolean { return false }

    @JvmStatic fun nativeBackupVerifyRecoveryMatchFallback(recoveryKey: String, authData: String): Boolean { return false }

    @JvmStatic fun nativeBase58EncodeFallback(data: ByteArray): String { return "" }

    @JvmStatic fun nativeBuildCallAnswerContentFallback(callId: String, sdpAnswer: String): String { return "" }

    @JvmStatic fun nativeBuildCallHangupContentFallback(callId: String, reason: String): String { return "" }

    @JvmStatic fun nativeBuildCallInviteContentFallback(callId: String, isVideo: Boolean, sdpOffer: String, lifetimeSec: Int): String { return "" }

    @JvmStatic fun nativeBuildCreateBackupBodyFallback(algorithm: String, authData: String): String { return "" }

    @JvmStatic fun nativeBuildDeviceDisplayNameFallback(appName: String, deviceModel: String): String { return "" }

    @JvmStatic fun nativeBuildDuckDuckGoUrlFallback(query: String): String { return "" }

    @JvmStatic fun nativeBuildEventPermalinkFallback(roomId: String, eventId: String): String { return "" }

    @JvmStatic fun nativeBuildGoogleUrlFallback(apiKey: String, engineId: String, query: String, maxResults: Int): String { return "" }

    @JvmStatic fun nativeBuildInviteBodyFallback(userId: String, reason: String): String { return "" }

    @JvmStatic fun nativeBuildKnockBodyFallback(reason: String): String { return "" }

    @JvmStatic fun nativeBuildMxcUriFallback(serverName: String, mediaId: String): String { return "" }

    @JvmStatic fun nativeBuildReplyRelationWithThreadFallback(eventId: String, threadRoot: String): String { return "" }

    @JvmStatic fun nativeBuildRoomNotifSettingsBodyFallback(mode: String): String { return "" }

    @JvmStatic fun nativeBuildRoomPermalinkFallback(roomId: String): String { return "" }

    @JvmStatic fun nativeBuildScanRequestBodyFallback(mxcUri: String): String { return "" }

    @JvmStatic fun nativeBuildSearxngUrlFallback(endpoint: String, query: String, maxResults: Int): String { return "" }

    @JvmStatic fun nativeBuildSessionRenameBodyFallback(sessionId: String, newName: String): String { return "" }

    @JvmStatic fun nativeBuildSpaceChildContentFallback(suggested: Boolean, order: String, autoJoin: Boolean, canonical: Boolean): String { return "" }

    @JvmStatic fun nativeBuildSpaceParentContentFallback(parentSpaceId: String, canonical: Boolean): String { return "" }

    @JvmStatic fun nativeBuildSsoLoginUrlFallback(baseUrl: String, redirectUrl: String): String { return "" }

    @JvmStatic fun nativeBuildSyncFilterFallback(includeThreads: Boolean, includePresence: Boolean, timelineLimit: Int, lazyLoadMembers: Boolean): String { return "" }

    @JvmStatic fun nativeBuildThreadListJsonFallback(eventsJson: String): String { return "" }

    @JvmStatic fun nativeBuildTosAcceptBodyFallback(version: String): String { return "" }

    @JvmStatic fun nativeBuildUserIdentifierFallback(userId: String): String { return "" }

    @JvmStatic fun nativeBuildUserPermalinkFallback(userId: String): String { return "" }

    @JvmStatic fun nativeBuildUserStatusJsonFallback(status: String, emoji: String, nowMs: Long): String { return "" }

    @JvmStatic fun nativeCacheEvictToFreeFallback(targetBytes: Long, availableBytes: Long, reservedBytes: Long): String { return "" }

    @JvmStatic fun nativeCacheGetPlanFallback(): String { return "" }

    @JvmStatic fun nativeCacheGetPressureFallback(availableBytes: Long, reservedBytes: Long): String { return "" }

    @JvmStatic fun nativeCacheGetStatsFallback(): String { return "" }

    @JvmStatic fun nativeCacheKeyForUrlFallback(url: String): String { return "" }

    @JvmStatic fun nativeCacheRecordHitFallback(): String { return "" }

    @JvmStatic fun nativeCacheRecordMissFallback(): String { return "" }

    @JvmStatic fun nativeCacheRegisterRoomFallback(): String { return "" }

    @JvmStatic fun nativeCallAnswerFallback(callId: String, sdpAnswer: String): String { return "" }

    @JvmStatic fun nativeCallFormatDurationFallback(seconds: Int): String { return "" }

    @JvmStatic fun nativeCallGetActiveFallback(): String { return "" }

    @JvmStatic fun nativeCallGetIncomingFallback(): String { return "" }

    @JvmStatic fun nativeCallGetRoomCallsFallback(roomId: String): String { return "" }

    @JvmStatic fun nativeCallHandleIncomingFallback(callId: String, roomId: String, callerId: String, callerName: String, callType: Int, sdpOffer: String, lifetimeSec: Int): String { return "" }

    @JvmStatic fun nativeCallHangupFallback(callId: String): String { return "" }

    @JvmStatic fun nativeCallIsRoomInCallFallback(roomId: String): Boolean { return false }

    @JvmStatic fun nativeCallParseSdpFallback(sdpText: String, type: String): String { return "" }

    @JvmStatic fun nativeCallRejectFallback(callId: String): String { return "" }

    @JvmStatic fun nativeCallResetFallback(): Boolean { return false }

    @JvmStatic fun nativeCallSetMutedFallback(): String { return "" }

    @JvmStatic fun nativeCallSetVideoFallback(): String { return "" }

    @JvmStatic fun nativeCallStartOutgoingFallback(roomId: String, calleeId: String, _calleeName: String, _callType: Int, sdpOffer: String): String { return "" }

    @JvmStatic fun nativeCanFitInStorageFallback(required: Long, available: Long, reserved: Long): Boolean { return false }

    @JvmStatic fun nativeCanReadMessagesFallback(membership: String): Boolean { return false }

    @JvmStatic fun nativeCanShareHistoryFallback(roomVisibility: String): Boolean { return false }

    @JvmStatic fun nativeCandidateAliasFromRoomNameFallback(roomName: String, domain: String, maxLength: Int): String { return "" }

    @JvmStatic fun nativeCanonicalizeJsonFallback(json: String): String { return "" }

    @JvmStatic fun nativeClassifyDeviceTypeFallback(userAgent: String, clientName: String): String { return "" }

    @JvmStatic fun nativeClassifyNetworkQualityFallback(signalStrength: Int, latencyMs: Double, lossRate: Double): String { return "" }

    @JvmStatic fun nativeClassifyWidgetTypeFallback(type: String): String { return "" }

    @JvmStatic fun nativeCompareSemverFallback(a: String, b: String): Int { return 0 }

    @JvmStatic fun nativeComposerApplyBoldFallback(text: String, selStart: Int, selEnd: Int): String { return "" }

    @JvmStatic fun nativeComposerApplyItalicFallback(text: String, selStart: Int, selEnd: Int): String { return "" }

    @JvmStatic fun nativeComposerAutoEmojiFallback(text: String): String { return "" }

    @JvmStatic fun nativeComposerBuildQuotedFallback(quotedText: String, replyText: String, quotedSender: String): String { return "" }

    @JvmStatic fun nativeComposerEnterEditFallback(): String { return "" }

    @JvmStatic fun nativeComposerEnterQuoteFallback(): String { return "" }

    @JvmStatic fun nativeComposerEnterRegularFallback(): String { return "" }

    @JvmStatic fun nativeComposerEnterReplyFallback(): String { return "" }

    @JvmStatic fun nativeComposerExtractMentionFallback(text: String, cursorPos: Int): String { return "" }

    @JvmStatic fun nativeComposerGetStateFallback(): String { return "" }

    @JvmStatic fun nativeComposerSetTextFallback(): String { return "" }

    @JvmStatic fun nativeComposerValidateFallback(text: String, maxLength: Int): String { return "" }

    @JvmStatic fun nativeComputeDeviceFingerprintFallback(identityKeyBase64: String): String { return "" }

    @JvmStatic fun nativeComputeEditDiffSummaryFallback(oldBody: String, newBody: String): String { return "" }

    @JvmStatic fun nativeComputeEncryptionStatusFallback(algorithm: String): String { return "" }

    @JvmStatic fun nativeComputeNotificationStateFallback(roomJson: String): String { return "" }

    @JvmStatic fun nativeComputePasswordStrengthFallback(password: String): Int { return 0 }

    @JvmStatic fun nativeComputePermissionsFallback(powerLevelsJson: String, myUserId: String): String { return "" }

    @JvmStatic fun nativeComputePollResultsFallback(pollJson: String): String { return "" }

    @JvmStatic fun nativeComputeRecoveryKeyFallback(curve25519Key: String): String { return "" }

    @JvmStatic fun nativeComputeThreadSummaryFallback(rootEventId: String, eventsJson: String): String { return "" }

    @JvmStatic fun nativeComputeThreadUnreadCountFallback(eventIdsJson: String, readReceiptId: String, highlightIdsJson: String): String { return "" }

    @JvmStatic fun nativeCountCharClassesFallback(password: String): Int { return 0 }

    @JvmStatic fun nativeCountEmojisFallback(text: String): Int { return 0 }

    @JvmStatic fun nativeCountEventsInSyncFallback(json: String): Int { return 0 }

    @JvmStatic fun nativeCountUniqueEmojisFallback(text: String): Int { return 0 }

    @JvmStatic fun nativeCreateRoomPresetToStringFallback(preset: Int): String { return "" }

    @JvmStatic fun nativeCreateUploadsFilterJsonFallback(numberOfEvents: Int): String { return "" }

    @JvmStatic fun nativeCredentialsToJsonFallback(credsJson: String): String { return "" }

    @JvmStatic fun nativeCrossSigningBuildKeysFallback(userId: String, mskPublic: String, uskPublic: String, sskPublic: String): String { return "" }

    @JvmStatic fun nativeCrossSigningCanSignFallback(): Boolean { return false }

    @JvmStatic fun nativeCrossSigningCheckSelfFallback(): String { return "" }

    @JvmStatic fun nativeCrossSigningImportKeysFallback(mskPrivate: String, uskPrivate: String, sskPrivate: String): String { return "" }

    @JvmStatic fun nativeCrossSigningIsInitFallback(): Boolean { return false }

    @JvmStatic fun nativeCrossSigningTrustMasterFallback(): String { return "" }

    @JvmStatic fun nativeDeviceBuildDeleteFallback(deviceId: String, authType: String, authSession: String, password: String): String { return "" }

    @JvmStatic fun nativeDeviceBuildRenameFallback(deviceId: String, newName: String): String { return "" }

    @JvmStatic fun nativeDeviceFormatFingerprintFallback(rawKey: String): String { return "" }

    @JvmStatic fun nativeDeviceFormatLastSeenFallback(timestampMs: Long): String { return "" }

    @JvmStatic fun nativeDeviceGetTrustLabelFallback(crossSigningVerified: Boolean, locallyVerified: Boolean): String { return "" }

    @JvmStatic fun nativeDeviceInfoToJsonFallback(deviceJson: String): String { return "" }

    @JvmStatic fun nativeDeviceIsInactiveFallback(lastSeenTs: Long, inactivityDays: Int): Boolean { return false }

    @JvmStatic fun nativeDeviceParseCryptoFallback(deviceId: String, userId: String, json: String): String { return "" }

    @JvmStatic fun nativeDeviceParseInfoFallback(deviceId: String, json: String): String { return "" }

    @JvmStatic fun nativeDeviceParseListFallback(json: String): String { return "" }

    @JvmStatic fun nativeDeviceSatisfiesVersionFallback(clientVersion: String, minRequired: String): Boolean { return false }

    @JvmStatic fun nativeDisambiguateNameFallback(displayName: String, mxid: String): String { return "" }

    @JvmStatic fun nativeDraftAutoSaveFallback(roomId: String, text: String): Boolean { return false }

    @JvmStatic fun nativeDraftDeleteFallback(): Boolean { return false }

    @JvmStatic fun nativeDraftGetFallback(roomId: String): String { return "" }

    @JvmStatic fun nativeDraftHasDraftFallback(roomId: String): Boolean { return false }

    @JvmStatic fun nativeDraftSaveFallback(): String { return "" }

    @JvmStatic fun nativeDraftStripPrefixFallback(text: String): String { return "" }

    @JvmStatic fun nativeEncryptedFileInfoToJsonFallback(infoJson: String): String { return "" }

    @JvmStatic fun nativeEncryptedFileKeyToJsonFallback(keyJson: String): String { return "" }

    @JvmStatic fun nativeEndCallReasonToStringFallback(reason: Int): String { return "" }

    @JvmStatic fun nativeEstimateMessageCacheSizeFallback(messageCount: Int, avgBodySize: Int): Long { return 0 }

    @JvmStatic fun nativeEvaluatePushNotificationFallback(eventJson: String, rulesJson: String, myDisplayName: String, myUserId: String): String { return "" }

    @JvmStatic fun nativeEventDistanceFallback(indexA: Int, indexB: Int): Int { return 0 }

    @JvmStatic fun nativeExtractAliasLocalpartFallback(alias: String): String { return "" }

    @JvmStatic fun nativeExtractCurveKeyFromRecoveryKeyFallback(recoveryKey: String): String { return "" }

    @JvmStatic fun nativeExtractDefaultSecretKeyFallback(accountDataJson: String): String { return "" }

    @JvmStatic fun nativeExtractDeviceFingerprintFallback(deviceId: String, keysJson: String): String { return "" }

    @JvmStatic fun nativeExtractEditSourceFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeExtractEventIdFromPermalinkFallback(url: String): String { return "" }

    @JvmStatic fun nativeExtractFileIvFallback(infoJson: String): String { return "" }

    @JvmStatic fun nativeExtractFileKeyFallback(keyJson: String): String { return "" }

    @JvmStatic fun nativeExtractHtmlTitleFallback(html: String): String { return "" }

    @JvmStatic fun nativeExtractMatrixIdsFallback(text: String): String { return "" }

    @JvmStatic fun nativeExtractMetaDescriptionFallback(html: String): String { return "" }

    @JvmStatic fun nativeExtractMxcMediaIdFallback(mxcUrl: String): String { return "" }

    @JvmStatic fun nativeExtractMxcServerNameFallback(mxcUrl: String): String { return "" }

    @JvmStatic fun nativeExtractNextBatchLightFallback(partialJson: String): String { return "" }

    @JvmStatic fun nativeExtractReplySourceFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeExtractRoomIdFromPermalinkFallback(url: String): String { return "" }

    @JvmStatic fun nativeExtractServerNameFromIdFallback(mxid: String): String { return "" }

    @JvmStatic fun nativeExtractSsoProviderFallback(idpId: String): String { return "" }

    @JvmStatic fun nativeExtractThreadRootFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeExtractUrlsFallback(text: String): String { return "" }

    @JvmStatic fun nativeExtractUsefulTextFromReplyFallback(repliedBody: String): String { return "" }

    @JvmStatic fun nativeExtractUserIdFromPermalinkFallback(url: String): String { return "" }

    @JvmStatic fun nativeExtractUserNameFromIdFallback(mxid: String): String { return "" }

    @JvmStatic fun nativeFederationVersionToJsonFallback(versionJson: String): String { return "" }

    @JvmStatic fun nativeFormatAudioNotificationFallback(sender: String, isVoice: Boolean): String { return "" }

    @JvmStatic fun nativeFormatBackupStatsFallback(infoJson: String): String { return "" }

    @JvmStatic fun nativeFormatBadgeTextFallback(totalCount: Int): String { return "" }

    @JvmStatic fun nativeFormatCallDurationFallback(seconds: Int): String { return "" }

    @JvmStatic fun nativeFormatCallNoticeFallback(eventType: String, isVideo: Boolean, senderName: String, sentBySelf: Boolean): String { return "" }

    @JvmStatic fun nativeFormatCallNotificationFallback(callJson: String): String { return "" }

    @JvmStatic fun nativeFormatCombinedNotificationCountFallback(roomCount: Int, threadCount: Int): String { return "" }

    @JvmStatic fun nativeFormatCrossSigningStatusFallback(statusJson: String): String { return "" }

    @JvmStatic fun nativeFormatDeviceLastSeenFallback(lastSeenMs: Long): String { return "" }

    @JvmStatic fun nativeFormatDowntimeFallback(downtimeMs: Long): String { return "" }

    @JvmStatic fun nativeFormatDurationFallback(durationMs: Long): String { return "" }

    @JvmStatic fun nativeFormatEditSummaryFallback(originalBody: String, newBody: String): String { return "" }

    @JvmStatic fun nativeFormatEventPreviewFallback(senderName: String, body: String, eventType: String, msgType: String, showSender: Boolean): String { return "" }

    @JvmStatic fun nativeFormatFileNotificationFallback(sender: String, fileName: String): String { return "" }

    @JvmStatic fun nativeFormatFileSizeFallback(bytes: Long): String { return "" }

    @JvmStatic fun nativeFormatFingerprintFallback(fingerprint: String): String { return "" }

    @JvmStatic fun nativeFormatImageNotificationFallback(sender: String): String { return "" }

    @JvmStatic fun nativeFormatInviteNotificationFallback(inviter: String, roomName: String): String { return "" }

    @JvmStatic fun nativeFormatKnockReasonFallback(reason: String): String { return "" }

    @JvmStatic fun nativeFormatLocationNotificationFallback(sender: String): String { return "" }

    @JvmStatic fun nativeFormatMediaCollapseLabelFallback(count: Int): String { return "" }

    @JvmStatic fun nativeFormatMemberNameFallback(displayName: String, userId: String, powerLevel: Int, showBadge: Boolean): String { return "" }

    @JvmStatic fun nativeFormatMemberNoticeFallback(membership: String, prevMembership: String, senderId: String, senderName: String, targetId: String, targetName: String, reason: String, isDirect: Boolean, sentBySelf: Boolean): String { return "" }

    @JvmStatic fun nativeFormatMembershipFallback(membership: String): String { return "" }

    @JvmStatic fun nativeFormatMsc3061StatusFallback(isShared: Boolean, visibilitySetting: String): String { return "" }

    @JvmStatic fun nativeFormatNotifModeFallback(mode: String): String { return "" }

    @JvmStatic fun nativeFormatOverflowLabelFallback(count: Int): String { return "" }

    @JvmStatic fun nativeFormatPollNotificationFallback(sender: String, isStart: Boolean): String { return "" }

    @JvmStatic fun nativeFormatPositionInfoFallback(positionMs: Long, durationMs: Long): String { return "" }

    @JvmStatic fun nativeFormatPowerLevelDiffFallback(senderName: String, oldLevelsJson: String, newLevelsJson: String, userNamesJson: String, sentBySelf: Boolean): String { return "" }

    @JvmStatic fun nativeFormatPresenceAggregationFallback(userNamesJson: String, maxNames: Int): String { return "" }

    @JvmStatic fun nativeFormatPresenceFallback(presence: String, lastActiveMs: Long): String { return "" }

    @JvmStatic fun nativeFormatReactionAggregationFallback(key: String, count: Int, reactorsJson: String): String { return "" }

    @JvmStatic fun nativeFormatReceiptAccessibilityFallback(receiptsJson: String, overflowCount: Int): String { return "" }

    @JvmStatic fun nativeFormatRecoveryKeyFallback(raw: String): String { return "" }

    @JvmStatic fun nativeFormatRedactionNoticeFallback(reason: String, redactedBySelf: Boolean, isStateEvent: Boolean): String { return "" }

    @JvmStatic fun nativeFormatRelationDescriptionFallback(relType: String, eventId: String, key: String): String { return "" }

    @JvmStatic fun nativeFormatRoomAvatarNoticeFallback(senderName: String, isRemoved: Boolean, sentBySelf: Boolean): String { return "" }

    @JvmStatic fun nativeFormatRoomCreateNoticeFallback(senderName: String, predecessorRoomId: String, isDirect: Boolean, sentBySelf: Boolean): String { return "" }

    @JvmStatic fun nativeFormatRoomEncryptionNoticeFallback(senderName: String, isEnabled: Boolean, sentBySelf: Boolean): String { return "" }

    @JvmStatic fun nativeFormatRoomNameNoticeFallback(senderName: String, newName: String, sentBySelf: Boolean): String { return "" }

    @JvmStatic fun nativeFormatRoomNotificationFallback(count: Int, roomName: String): String { return "" }

    @JvmStatic fun nativeFormatRoomTombstoneNoticeFallback(senderName: String, replacementRoom: String, sentBySelf: Boolean): String { return "" }

    @JvmStatic fun nativeFormatRoomTopicNoticeFallback(senderName: String, newTopic: String, sentBySelf: Boolean): String { return "" }

    @JvmStatic fun nativeFormatSpoilerTextFromHtmlFallback(formattedBody: String): String { return "" }

    @JvmStatic fun nativeFormatStatusMessageFallback(message: String, maxLen: Int): String { return "" }

    @JvmStatic fun nativeFormatStickerNotificationFallback(sender: String): String { return "" }

    @JvmStatic fun nativeFormatThreadNotificationCountFallback(threadCount: Int, highlightCount: Int): String { return "" }

    @JvmStatic fun nativeFormatTimeAgoLabelFallback(timestampMs: Long, nowMs: Long): String { return "" }

    @JvmStatic fun nativeFormatUnreadCounterFallback(count: Int): String { return "" }

    @JvmStatic fun nativeFormatVideoNotificationFallback(sender: String): String { return "" }

    @JvmStatic fun nativeGenerateDeviceIdFallback(): String { return "" }

    @JvmStatic fun nativeGenerateDeviceNameFallback(model: String, osVersion: String): String { return "" }

    @JvmStatic fun nativeGeneratePasswordFeedbackFallback(password: String): String { return "" }

    @JvmStatic fun nativeGeneratePollOptionIdFallback(): String { return "" }

    @JvmStatic fun nativeGetAllErrorCodesFallback(): String { return "" }

    @JvmStatic fun nativeGetBackupAlgorithmDescriptionFallback(algorithm: String): String { return "" }

    @JvmStatic fun nativeGetBannerColorFallback(downtimeMs: Long): String { return "" }

    @JvmStatic fun nativeGetBestDisplayNameFallback(displayName: String, userId: String): String { return "" }

    @JvmStatic fun nativeGetCallStateFallback(eventContentJson: String): String { return "" }

    @JvmStatic fun nativeGetDefaultEncryptionAlgorithmFallback(): String { return "" }

    @JvmStatic fun nativeGetDefaultModeForRoomFallback(isDirect: Boolean, isEncrypted: Boolean): String { return "" }

    @JvmStatic fun nativeGetE2eeColorFallback(state: String): String { return "" }

    @JvmStatic fun nativeGetE2eeIconNameFallback(state: String): String { return "" }

    @JvmStatic fun nativeGetEditBadgeTextFallback(editCount: Int): String { return "" }

    @JvmStatic fun nativeGetEditCountBadgeFallback(editCount: Int): String { return "" }

    @JvmStatic fun nativeGetEditedTargetEventIdFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeGetErrorDescriptionFallback(errorCode: String): String { return "" }

    @JvmStatic fun nativeGetEventTypeDescriptionFallback(eventType: String, msgType: String): String { return "" }

    @JvmStatic fun nativeGetEventTypeIconFallback(eventType: String, msgType: String): String { return "" }

    @JvmStatic fun nativeGetExtensionFromMimeTypeFallback(mimetype: String): String { return "" }

    @JvmStatic fun nativeGetIdentityInitialsFallback(displayName: String): String { return "" }

    @JvmStatic fun nativeGetInitialsFallback(name: String, maxChars: Int): String { return "" }

    @JvmStatic fun nativeGetLatestEditEventIdFallback(editSummaryJson: String, originalEventId: String): String { return "" }

    @JvmStatic fun nativeGetMinPassphraseLengthFallback(): Int { return 0 }

    @JvmStatic fun nativeGetNextBatchFallback(json: String): String { return "" }

    @JvmStatic fun nativeGetPresenceIndicatorFallback(presence: String): String { return "" }

    @JvmStatic fun nativeGetPresenceStatusTextFallback(isOnline: Boolean, lastActiveMs: Long): String { return "" }

    @JvmStatic fun nativeGetReasonDescriptionFallback(code: String): String { return "" }

    @JvmStatic fun nativeGetRecoveryKeyExampleFallback(): String { return "" }

    @JvmStatic fun nativeGetRetryAfterMsFallback(_errorJson: String): Long { return 0 }

    @JvmStatic fun nativeGetRuleKindDescriptionFallback(kind: String, enabled: Boolean): String { return "" }

    @JvmStatic fun nativeGetSettingBoolFallback(settingsJson: String, key: String, defaultVal: Boolean): Boolean { return false }

    @JvmStatic fun nativeGetSettingStringFallback(settingsJson: String, key: String, defaultVal: String): String { return "" }

    @JvmStatic fun nativeGetSsoProviderBrandFallback(provider: String): String { return "" }

    @JvmStatic fun nativeGetStatusSuggestionsFallback(): String { return "" }

    @JvmStatic fun nativeGetStrengthLabelFallback(strength: Int): String { return "" }

    @JvmStatic fun nativeGetTotalUnreadCountFallback(roomCount: Int, threadCount: Int): Int { return 0 }

    @JvmStatic fun nativeGetTrustLabelFallback(level: String): String { return "" }

    @JvmStatic fun nativeGetWidgetTypeNameFallback(type: String): String { return "" }

    @JvmStatic fun nativeHasAttachmentUrlFallback(decryptedContentJson: String): Boolean { return false }

    @JvmStatic fun nativeHasCrossSigningSecretsFallback(accountDataJson: String): Boolean { return false }

    @JvmStatic fun nativeHasPowerFallback(plJson: String, userId: String, action: String): Boolean { return false }

    @JvmStatic fun nativeHasTextWithImageFallback(contentJson: String): Boolean { return false }

    @JvmStatic fun nativeHistoryVisibilityToStringFallback(stateContentJson: String): String { return "" }

    @JvmStatic fun nativeIdentityBuildBindFallback(threePid: String): String { return "" }

    @JvmStatic fun nativeIdentityBuildLookupFallback(pidsJson: String): String { return "" }

    @JvmStatic fun nativeIdentityGetServerFallback(): String { return "" }

    @JvmStatic fun nativeIdentityParse3pidFallback(input: String): String { return "" }

    @JvmStatic fun nativeIdentityParseLookupFallback(json: String): String { return "" }

    @JvmStatic fun nativeIdentitySetServerFallback(url: String): String { return "" }

    @JvmStatic fun nativeIsActiveMemberFallback(membership: String): Boolean { return false }

    @JvmStatic fun nativeIsAppPermalinkFallback(url: String): Boolean { return false }

    @JvmStatic fun nativeIsAutoApprovedCapabilityFallback(capability: Int, widgetType: String): Boolean { return false }

    @JvmStatic fun nativeIsBodyWithinLimitsFallback(body: String, maxLength: Int): Boolean { return false }

    @JvmStatic fun nativeIsCallExpiredFallback(createdAtMs: Long, timeoutSec: Int): Boolean { return false }

    @JvmStatic fun nativeIsCanonicalAliasFallback(alias: String, expectedRoomId: String): Boolean { return false }

    @JvmStatic fun nativeIsCommonPasswordFallback(password: String): Boolean { return false }

    @JvmStatic fun nativeIsContentScannerAvailableFallback(serverCapabilitiesJson: String): Boolean { return false }

    @JvmStatic fun nativeIsContinuationFallback(curSender: String, prevSender: String, curTs: Long, prevTs: Long): Boolean { return false }

    @JvmStatic fun nativeIsDeviceInactiveFallback(lastSeenMs: Long): Boolean { return false }

    @JvmStatic fun nativeIsEditFallback(contentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsEmailFallback(input: String): Boolean { return false }

    @JvmStatic fun nativeIsEmojiCodePointFallback(codepoint: Int): Boolean { return false }

    @JvmStatic fun nativeIsEtherpadWidgetFallback(type: String): Boolean { return false }

    @JvmStatic fun nativeIsEventIdFallback(input: String): Boolean { return false }

    @JvmStatic fun nativeIsFileSizeWithinLimitsFallback(fileSize: Long, maxSizeBytes: Long): Boolean { return false }

    @JvmStatic fun nativeIsGroupIdFallback(input: String): Boolean { return false }

    @JvmStatic fun nativeIsHistoryPubliclyVisibleFallback(stateContentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsImageUrlFallback(url: String): Boolean { return false }

    @JvmStatic fun nativeIsInviteExpiredFallback(invitedAtMs: Long, maxAgeDays: Int): Boolean { return false }

    @JvmStatic fun nativeIsInviteOnlyFallback(stateContentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsJitsiWidgetFallback(type: String): Boolean { return false }

    @JvmStatic fun nativeIsKnownPushRuleKindFallback(kind: String): Boolean { return false }

    @JvmStatic fun nativeIsMatrixToPermalinkFallback(url: String): Boolean { return false }

    @JvmStatic fun nativeIsMsc3061SharedKeyFallback(roomKeyContentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsMsisdnFallback(input: String): Boolean { return false }

    @JvmStatic fun nativeIsMxcUriFallback(url: String): Boolean { return false }

    @JvmStatic fun nativeIsMxcUrlFallback(url: String): Boolean { return false }

    @JvmStatic fun nativeIsNotifModeDifferentFallback(oldMode: String, newMode: String): Boolean { return false }

    @JvmStatic fun nativeIsOffensiveFallback(score: Int): Boolean { return false }

    @JvmStatic fun nativeIsPasswordErrorFallback(errorCode: String): Boolean { return false }

    @JvmStatic fun nativeIsPhoneNumberFallback(input: String): Boolean { return false }

    @JvmStatic fun nativeIsPollEndedFallback(closeTimestampMs: Long): Boolean { return false }

    @JvmStatic fun nativeIsPresenceStaleFallback(lastUpdatedMs: Long): Boolean { return false }

    @JvmStatic fun nativeIsPreviewableUrlFallback(url: String): Boolean { return false }

    @JvmStatic fun nativeIsPublicRoomFallback(stateContentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsReactionFallback(contentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsReasonableTimestampFallback(originServerTs: String, maxFutureMs: Long): Boolean { return false }

    @JvmStatic fun nativeIsReplyFallback(contentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsRoomAliasFallback(input: String): Boolean { return false }

    @JvmStatic fun nativeIsRoomEncryptedFallback(stateContentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsRoomIdFallback(input: String): Boolean { return false }

    @JvmStatic fun nativeIsRoomUpgradedFallback(stateContentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsSameRoomPermalinkFallback(url1: String, url2: String): Boolean { return false }

    @JvmStatic fun nativeIsServerCompatibleFallback(serverVersion: String, minRequired: String): Boolean { return false }

    @JvmStatic fun nativeIsServerNoticeFallback(eventContentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsSsoCallbackUrlFallback(url: String): Boolean { return false }

    @JvmStatic fun nativeIsStateEventFallback(eventType: String): Boolean { return false }

    @JvmStatic fun nativeIsStickerEventFallback(eventType: String): Boolean { return false }

    @JvmStatic fun nativeIsSupportedAudioTypeFallback(mimeType: String): Boolean { return false }

    @JvmStatic fun nativeIsSupportedBackupAlgorithmFallback(algorithm: String): Boolean { return false }

    @JvmStatic fun nativeIsThreadRootFallback(contentJson: String): Boolean { return false }

    @JvmStatic fun nativeIsUserIdFallback(input: String): Boolean { return false }

    @JvmStatic fun nativeIsValidDeviceKeyFallback(key: String): Boolean { return false }

    @JvmStatic fun nativeIsValidDisplayNameFallback(name: String, maxLen: Int): Boolean { return false }

    @JvmStatic fun nativeIsValidEmailFallback(input: String): Boolean { return false }

    @JvmStatic fun nativeIsValidEncryptedFileFallback(infoJson: String): Boolean { return false }

    @JvmStatic fun nativeIsValidEventIdFallback(eventId: String): Boolean { return false }

    @JvmStatic fun nativeIsValidJwkKeyFallback(keyJson: String): Boolean { return false }

    @JvmStatic fun nativeIsValidLoginCredentialsFallback(userId: String, password: String): Boolean { return false }

    @JvmStatic fun nativeIsValidOrderStringFallback(order: String): Boolean { return false }

    @JvmStatic fun nativeIsValidPassphraseFallback(passphrase: String): Boolean { return false }

    @JvmStatic fun nativeIsValidPollQuestionFallback(question: String): Boolean { return false }

    @JvmStatic fun nativeIsValidReportReasonFallback(reason: String): Boolean { return false }

    @JvmStatic fun nativeIsValidSenderIdFallback(senderId: String): Boolean { return false }

    @JvmStatic fun nativeIsValidUserIdFallback(userId: String): Boolean { return false }

    @JvmStatic fun nativeIsValidWidgetUrlFallback(url: String): Boolean { return false }

    @JvmStatic fun nativeJoinRuleToStringFallback(stateContentJson: String): String { return "" }

    @JvmStatic fun nativeListRoomWidgetsFallback(stateEventsJson: String): String { return "" }

    @JvmStatic fun nativeLiveDraftConfigToJsonFallback(configJson: String): String { return "" }

    @JvmStatic fun nativeLiveLocationBuildMapUrlFallback(roomId: String, configJson: String): String { return "" }

    @JvmStatic fun nativeLiveLocationClusterFallback(coordsJson: String, radiusMeters: Double): String { return "" }

    @JvmStatic fun nativeLiveLocationDistanceFallback(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double { return 0.0 }

    @JvmStatic fun nativeLiveLocationFormatGeoUriFallback(lat: Double, lon: Double): String { return "" }

    @JvmStatic fun nativeLiveLocationFormatMessageFallback(lat: Double, lon: Double, accuracy: Double, label: String): String { return "" }

    @JvmStatic fun nativeLiveLocationGetActiveFallback(userId: String): String { return "" }

    @JvmStatic fun nativeLiveLocationGetRoomSessionsFallback(roomId: String): String { return "" }

    @JvmStatic fun nativeLiveLocationHistoryFallback(sessionId: String): String { return "" }

    @JvmStatic fun nativeLiveLocationIsDueFallback(sessionId: String): Boolean { return false }

    @JvmStatic fun nativeLiveLocationParseGeoUriFallback(uri: String): String { return "" }

    @JvmStatic fun nativeLiveLocationStartSessionFallback(roomId: String, userId: String, description: String, timeoutSec: Int, intervalSec: Int, autoStop: Boolean, autoStopMin: Int): String { return "" }

    @JvmStatic fun nativeLiveLocationStopSessionFallback(sessionId: String): String { return "" }

    @JvmStatic fun nativeLiveLocationUpdateFallback(sessionId: String, lat: Double, lon: Double, accuracy: Double): String { return "" }

    @JvmStatic fun nativeLiveLocationWithinGeofenceFallback(lat: Double, lon: Double, centerLat: Double, centerLon: Double, radiusMeters: Double): Boolean { return false }

    @JvmStatic fun nativeLocationExportJsonFallback(): String { return "" }

    @JvmStatic fun nativeLocationIsDueFallback(sessionId: String): Boolean { return false }

    @JvmStatic fun nativeLocationStartSessionFallback(sessionId: String, roomId: String, userId: String, intervalSec: Int): String { return "" }

    @JvmStatic fun nativeLocationStopSessionFallback(): Boolean { return false }

    @JvmStatic fun nativeMarkdownToHtmlFallback(markdown: String, enableTables: Boolean, enableLinks: Boolean, enableCode: Boolean, enableScroll: Boolean): String { return "" }

    @JvmStatic fun nativeMediaViewerCanThumbnailFallback(mimeType: String): Boolean { return false }

    @JvmStatic fun nativeMediaViewerDownloadUrlFallback(mxcUrl: String, homeServer: String): String { return "" }

    @JvmStatic fun nativeMediaViewerExifRotationFallback(rawExif: Int): Int { return 0 }

    @JvmStatic fun nativeMediaViewerFormatDurationFallback(durationMs: Int): String { return "" }

    @JvmStatic fun nativeMediaViewerFormatSizeFallback(bytes: Long): String { return "" }

    @JvmStatic fun nativeMediaViewerParseFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeMediaViewerThumbnailUrlFallback(mxcUrl: String, homeServer: String, width: Int, height: Int): String { return "" }

    @JvmStatic fun nativeMediaViewerViewportFallback(contentJson: String, viewportW: Int, viewportH: Int): String { return "" }

    @JvmStatic fun nativeMeetsMinimumRequirementsFallback(password: String): Boolean { return false }

    @JvmStatic fun nativeMegolmAddSessionFallback(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String): Boolean { return false }

    @JvmStatic fun nativeMegolmClearRoomFallback(): Boolean { return false }

    @JvmStatic fun nativeMegolmDecryptFallback(roomId: String, senderKey: String, sessionId: String, ciphertext: String): String { return "" }

    @JvmStatic fun nativeMegolmSessionCountFallback(): Int { return 0 }

    @JvmStatic fun nativeMessageAudioToJsonFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeMessageEmoteToJsonFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeMessageFileToJsonFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeMessageImageToJsonFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeMessageNoticeToJsonFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeMessageTextToJsonFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeMessageVideoToJsonFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeMimeToMsgTypeFallback(mimeType: String): String { return "" }

    @JvmStatic fun nativeMustAcceptTosFallback(responseJson: String): Boolean { return false }

    @JvmStatic fun nativeNeedsBackupAttentionFallback(infoJson: String): Boolean { return false }

    @JvmStatic fun nativeNeedsCrossSigningSetupFallback(statusJson: String): Boolean { return false }

    @JvmStatic fun nativeNeedsWellKnownDiscoveryFallback(homeserverUrl: String): Boolean { return false }

    @JvmStatic fun nativeNormalizeMimeTypeFallback(mimeType: String): String { return "" }

    @JvmStatic fun nativeOidcBuildAuthorizationFallback(metadataJson: String, registrationJson: String, configJson: String): String { return "" }

    @JvmStatic fun nativeOidcBuildPasswordLoginFallback(userId: String, password: String, deviceId: String, deviceName: String): String { return "" }

    @JvmStatic fun nativeOidcBuildRefreshFallback(refreshToken: String, clientId: String): String { return "" }

    @JvmStatic fun nativeOidcBuildRegistrationFallback(configJson: String): String { return "" }

    @JvmStatic fun nativeOidcExtractCodeFallback(callbackUrl: String): String { return "" }

    @JvmStatic fun nativeOidcIsCallbackFallback(url: String): Boolean { return false }

    @JvmStatic fun nativeOidcParseMetadataFallback(json: String): String { return "" }

    @JvmStatic fun nativeOidcParseRegistrationFallback(json: String): String { return "" }

    @JvmStatic fun nativeOidcParseTokenFallback(json: String): String { return "" }

    @JvmStatic fun nativeOidcParseWellKnownFallback(json: String): String { return "" }

    @JvmStatic fun nativeOidcParseWhoamiFallback(json: String): String { return "" }

    @JvmStatic fun nativeOlmCreateAccountFallback(userId: String, deviceId: String): Boolean { return false }

    @JvmStatic fun nativeOlmCreateInboundSessionFallback(theirIdentityKey: String, preKeyMessage: String): String { return "" }

    @JvmStatic fun nativeOlmDecryptMessageFallback(senderKey: String, sessionId: String, ciphertext: String): String { return "" }

    @JvmStatic fun nativeOlmGenerateOneTimeKeysFallback(count: Int): String { return "" }

    @JvmStatic fun nativeOlmGetIdentityKeysFallback(): String { return "" }

    @JvmStatic fun nativeOlmPickleAccountFallback(): String { return "" }

    @JvmStatic fun nativeOlmSignMessageFallback(message: String): String { return "" }

    @JvmStatic fun nativeOlmUnpickleAccountFallback(pickled: String, userId: String, deviceId: String): Boolean { return false }

    @JvmStatic fun nativeOverlayBackFallback(timeNs: Long): Int { return 0 }

    @JvmStatic fun nativeOverlayGetStateFallback(): String { return "" }

    @JvmStatic fun nativeOverlayIsTouchAllowedFallback(action: Int): Boolean { return false }

    @JvmStatic fun nativeOverlaySafetyToJsonFallback(): String { return "" }

    @JvmStatic fun nativeOverlaySetConfigFallback(): Int { return 0 }

    @JvmStatic fun nativeOverlaySetSafetyModeFallback(): String { return "" }

    @JvmStatic fun nativeOverlaySetSafetyPermsFallback(): Boolean { return false }

    @JvmStatic fun nativeOverlayTickFallback(timeNs: Long): Int { return 0 }

    @JvmStatic fun nativeOverlayTouchDownFallback(x: Double, y: Double, pointerId: Int, timeNs: Long): Int { return 0 }

    @JvmStatic fun nativeOverlayTouchMoveFallback(x: Double, y: Double, pointerId: Int, timeNs: Long): Int { return 0 }

    @JvmStatic fun nativeOverlayTouchUpFallback(pointerId: Int, timeNs: Long): Int { return 0 }

    @JvmStatic fun nativeParseBackupInfoFallback(apiResponseJson: String): String { return "" }

    @JvmStatic fun nativeParseCrossSigningStatusFallback(accountDataJson: String, userId: String): String { return "" }

    @JvmStatic fun nativeParseDeviceListFallback(apiResponseJson: String, currentDeviceId: String): String { return "" }

    @JvmStatic fun nativeParseDeviceNameFallback(userAgent: String): String { return "" }

    @JvmStatic fun nativeParseDirectMessageMapFallback(json: String): String { return "" }

    @JvmStatic fun nativeParseEncryptionConfigFallback(stateContentJson: String): String { return "" }

    @JvmStatic fun nativeParseEventContentFallback(eventType: String, contentJson: String): String { return "" }

    @JvmStatic fun nativeParseEventFallback(json: String): String { return "" }

    @JvmStatic fun nativeParseEventRelationFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeParseFederationVersionFallback(json: String): String { return "" }

    @JvmStatic fun nativeParseGuestAccessFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeParseHistoryVisibilityFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeParseJoinRulesFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeParseJsonStringValueFallback(json: String, key: String): String { return "" }

    @JvmStatic fun nativeParseKeyBackupVersionFallback(json: String): String { return "" }

    @JvmStatic fun nativeParseLoginFlowsListFallback(apiResponseJson: String): String { return "" }

    @JvmStatic fun nativeParseMarkdownTableFallback(tableBlock: String, withScroll: Boolean): String { return "" }

    @JvmStatic fun nativeParseMatrixToPermalinkFallback(url: String): String { return "" }

    @JvmStatic fun nativeParseMemberListFallback(roomId: String, apiResponseJson: String, isTruncated: Boolean): String { return "" }

    @JvmStatic fun nativeParseNotifModeFallback(action: String): String { return "" }

    @JvmStatic fun nativeParseOpenIdTokenFallback(json: String): String { return "" }

    @JvmStatic fun nativeParsePresenceFallback(userId: String, apiResponseJson: String): String { return "" }

    @JvmStatic fun nativeParsePresenceInfoFallback(userId: String, apiResponseJson: String): String { return "" }

    @JvmStatic fun nativeParsePublicRoomFallback(json: String): String { return "" }

    @JvmStatic fun nativeParsePublicRoomsResponseFallback(json: String): String { return "" }

    @JvmStatic fun nativeParseRoomAvatarContentFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeParseRoomNameContentFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeParseRoomPowerLevelsFallback(stateContentJson: String): String { return "" }

    @JvmStatic fun nativeParseRoomTombstoneContentFallback(stateEventJson: String): String { return "" }

    @JvmStatic fun nativeParseRoomTopicContentFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeParseScanResultFallback(apiResponseJson: String): String { return "" }

    @JvmStatic fun nativeParseServerNoticeFallback(eventContentJson: String, eventId: String): String { return "" }

    @JvmStatic fun nativeParseServerVersionFallback(apiResponseJson: String): String { return "" }

    @JvmStatic fun nativeParseSpaceChildrenFallback(stateEventsJson: String): String { return "" }

    @JvmStatic fun nativeParseSyncResponseFallback(json: String): String { return "" }

    @JvmStatic fun nativeParseSyncRoomsJsonFallback(json: String): String { return "" }

    @JvmStatic fun nativeParseThreePidFallback(input: String): String { return "" }

    @JvmStatic fun nativeParseTimelineFallback(json: String): String { return "" }

    @JvmStatic fun nativeParseTombstoneFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeParseUrlFallback(url: String): String { return "" }

    @JvmStatic fun nativeParseWellKnownFallback(responseJson: String): String { return "" }

    @JvmStatic fun nativeParseWidgetStateContentFallback(stateContentJson: String, widgetId: String, roomId: String): String { return "" }

    @JvmStatic fun nativePermBuildBanFallback(userId: String, reason: String): String { return "" }

    @JvmStatic fun nativePermBuildContentFallback(powerLevelsJson: String): String { return "" }

    @JvmStatic fun nativePermBuildKickFallback(userId: String, reason: String): String { return "" }

    @JvmStatic fun nativePermFormatChangeFallback(userId: String, oldPower: Int, newPower: Int): String { return "" }

    @JvmStatic fun nativePermGetRoleFallback(userId: String, powerLevel: Int): String { return "" }

    @JvmStatic fun nativePermParseFallback(powerLevelsJson: String): String { return "" }

    @JvmStatic fun nativePinCanManageFallback(powerLevel: Int): Boolean { return false }

    @JvmStatic fun nativePinCountFallback(roomId: String): Int { return 0 }

    @JvmStatic fun nativePinEventFallback(roomId: String, eventId: String, pinnedBy: String, powerLevel: Int): String { return "" }

    @JvmStatic fun nativePinGetEventsFallback(roomId: String): String { return "" }

    @JvmStatic fun nativePinIsPinnedFallback(roomId: String, eventId: String): Boolean { return false }

    @JvmStatic fun nativePinLoadStateFallback(): String { return "" }

    @JvmStatic fun nativePinResetFallback(): String { return "" }

    @JvmStatic fun nativePinToggleFallback(roomId: String, eventId: String, userId: String, powerLevel: Int): String { return "" }

    @JvmStatic fun nativePollBuildEndFallback(pollId: String, reason: String, unstable: Boolean): String { return "" }

    @JvmStatic fun nativePollBuildResponseFallback(pollId: String, selectionsJson: String, unstable: Boolean): String { return "" }

    @JvmStatic fun nativePollBuildStartFallback(question: String, optionsJson: String, kind: Int, maxSelections: Int, unstable: Boolean): String { return "" }

    @JvmStatic fun nativePollIsValidQuestionFallback(question: String): Boolean { return false }

    @JvmStatic fun nativePollTallyFallback(pollJson: String, votesJson: String): String { return "" }

    @JvmStatic fun nativePollTypeFromStringFallback(type: String): Int { return 0 }

    @JvmStatic fun nativePollTypeToStringFallback(type: Int): String { return "" }

    @JvmStatic fun nativePresenceEnumToStringFallback(presence: Int): String { return "" }

    @JvmStatic fun nativeProcessRoomUpgradeFallback(tombstoneEventJson: String): String { return "" }

    @JvmStatic fun nativeProfileActionReportFallback(): String { return "" }

    @JvmStatic fun nativeProfileActionReportTextFallback(): String { return "" }

    @JvmStatic fun nativeProfileGetSummaryFallback(name: String): String { return "" }

    @JvmStatic fun nativeProfileIsActiveFallback(): Boolean { return false }

    @JvmStatic fun nativeProfileMemoryFallback(): String { return "" }

    @JvmStatic fun nativeProfileOverlaySnapshotFallback(): String { return "" }

    @JvmStatic fun nativeProfileOverlayTextFallback(): String { return "" }

    @JvmStatic fun nativeProfileReportFallback(): String { return "" }

    @JvmStatic fun nativeProfileReportTextFallback(): String { return "" }

    @JvmStatic fun nativeProfileResetFallback(): Boolean { return false }

    @JvmStatic fun nativeProfileSetBudgetFallback(): String { return "" }

    @JvmStatic fun nativeProfileStartActionFallback(actionName: String, isCold: Boolean): Int { return 0 }

    @JvmStatic fun nativeProfileStartFallback(): Boolean { return false }

    @JvmStatic fun nativeProfileStopActionFallback(actionIndex: Int): Long { return 0 }

    @JvmStatic fun nativeProfileStopFallback(): Boolean { return false }

    @JvmStatic fun nativeRelationBuildAnnotationFallback(eventId: String, key: String): String { return "" }

    @JvmStatic fun nativeRelationBuildEditFallback(eventId: String): String { return "" }

    @JvmStatic fun nativeRelationBuildReplyFallback(eventId: String): String { return "" }

    @JvmStatic fun nativeRelationBuildThreadFallback(eventId: String, replyToId: String): String { return "" }

    @JvmStatic fun nativeRelationExtractReplySourceFallback(eventContent: String): String { return "" }

    @JvmStatic fun nativeRelationExtractThreadRootFallback(eventContent: String): String { return "" }

    @JvmStatic fun nativeRelationIsEditFallback(eventContent: String): Boolean { return false }

    @JvmStatic fun nativeRelationIsReactionFallback(eventContent: String): Boolean { return false }

    @JvmStatic fun nativeRelationIsReplyFallback(eventContent: String): Boolean { return false }

    @JvmStatic fun nativeRelationParseFallback(eventContent: String): String { return "" }

    @JvmStatic fun nativeRequiresDeviceVerificationFallback(algorithm: String): Boolean { return false }

    @JvmStatic fun nativeResolveMxcDownloadUrlFallback(mxcUrl: String, homeServerUrl: String): String { return "" }

    @JvmStatic fun nativeResolveMxcThumbnailUrlFallback(mxcUrl: String, homeServerUrl: String, width: Int, height: Int): String { return "" }

    @JvmStatic fun nativeResolveUrlFallback(baseUrl: String, relative: String): String { return "" }

    @JvmStatic fun nativeRoomDirBuildSearchFallback(searchTerm: String, limit: Int, since: String): String { return "" }

    @JvmStatic fun nativeRoomDirBuildVisibilityFallback(visibility: Int): String { return "" }

    @JvmStatic fun nativeRoomDirCheckAliasFallback(aliasLocalPart: String, json: String): String { return "" }

    @JvmStatic fun nativeRoomDirFormatPreviewFallback(roomJson: String): String { return "" }

    @JvmStatic fun nativeRoomDirParseResponseFallback(json: String): String { return "" }

    @JvmStatic fun nativeRoomDirParseVisibilityFallback(json: String): String { return "" }

    @JvmStatic fun nativeRoomStateIsInviteOnlyFallback(roomId: String): Boolean { return false }

    @JvmStatic fun nativeRoomStateIsPublicFallback(roomId: String): Boolean { return false }

    @JvmStatic fun nativeRoomStateParseJoinRulesFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeRoomStateParseVisibilityFallback(contentJson: String): String { return "" }

    @JvmStatic fun nativeRoomStateSetJoinRuleFallback(): String { return "" }

    @JvmStatic fun nativeRoomStateSetVisibilityFallback(): String { return "" }

    @JvmStatic fun nativeRoomStateShouldShareFallback(contentJson: String): Boolean { return false }

    @JvmStatic fun nativeSasCalculateMacFallback(input: String, info: String): String { return "" }

    @JvmStatic fun nativeSasCreateFallback(): String { return "" }

    @JvmStatic fun nativeSasDestroyFallback(): String { return "" }

    @JvmStatic fun nativeSasGetEmojisFallback(): String { return "" }

    @JvmStatic fun nativeSasSetTheirKeyFallback(theirPubkey: String): Boolean { return false }

    @JvmStatic fun nativeSasVerifyMacFallback(theirMac: String, input: String, info: String): Boolean { return false }

    @JvmStatic fun nativeSatisfiesMinVersionFallback(current: String, minimum: String): Boolean { return false }

    @JvmStatic fun nativeSdpTypeToStringFallback(type: Int): String { return "" }

    @JvmStatic fun nativeSearchRoomListFallback(roomsJson: String, query: String): String { return "" }

    @JvmStatic fun nativeSearchSpaceChildrenFallback(childrenJson: String, query: String): String { return "" }

    @JvmStatic fun nativeServerNoticeFormatDowntimeFallback(retryAfterMs: Long): String { return "" }

    @JvmStatic fun nativeServerNoticeFormatLimitFallback(_errorJson: String, mode: Int): String { return "" }

    @JvmStatic fun nativeServerNoticeGetBannerFallback(errorJson: String): String { return "" }

    @JvmStatic fun nativeServerNoticeGetDescriptionFallback(errorCode: String): String { return "" }

    @JvmStatic fun nativeServerNoticeIsConsentFallback(errorCode: String): Boolean { return false }

    @JvmStatic fun nativeServerNoticeIsRateLimitFallback(errorCode: String): Boolean { return false }

    @JvmStatic fun nativeServerNoticeIsResourceLimitFallback(errorCode: String): Boolean { return false }

    @JvmStatic fun nativeServerNoticeParseFallback(errorJson: String): String { return "" }

    @JvmStatic fun nativeSessionCloseFallback(sessionId: String): Boolean { return false }

    @JvmStatic fun nativeSessionComputeIdFallback(userId: String, deviceId: String): String { return "" }

    @JvmStatic fun nativeSessionCountFallback(): Int { return 0 }

    @JvmStatic fun nativeSessionCreateFallback(credentialsJson: String, configJson: String, loginType: Int): String { return "" }

    @JvmStatic fun nativeSessionGetActiveFallback(): String { return "" }

    @JvmStatic fun nativeSessionGetAllFallback(): String { return "" }

    @JvmStatic fun nativeSessionHasActiveFallback(): Boolean { return false }

    @JvmStatic fun nativeSessionOpenFallback(sessionId: String): Boolean { return false }

    @JvmStatic fun nativeSessionRemoveFallback(sessionId: String): Boolean { return false }

    @JvmStatic fun nativeSessionSetActiveFallback(sessionId: String): Boolean { return false }

    @JvmStatic fun nativeSetAccessTokenFallback(): String { return "" }

    @JvmStatic fun nativeSetHomeserverUrlFallback(): String { return "" }

    @JvmStatic fun nativeSetSettingBoolFallback(settingsJson: String, key: String, value: Boolean): String { return "" }

    @JvmStatic fun nativeSetSettingStringFallback(settingsJson: String, key: String, value: String): String { return "" }

    @JvmStatic fun nativeSha256Fallback(data: ByteArray): String { return "" }

    @JvmStatic fun nativeShouldIgnoreSignOutErrorFallback(errorCode: String, httpCode: Int): Boolean { return false }

    @JvmStatic fun nativeShouldShareKeyFallback(algorithm: String, hasSession: Boolean, sessionVerified: Boolean, userTrusted: Boolean): Boolean { return false }

    @JvmStatic fun nativeShouldShowTimestampFallback(currentSender: String, currentTs: Long, previousTs: Long, showAll: Boolean): Boolean { return false }

    @JvmStatic fun nativeSignEventFallback(eventJson: String): String { return "" }

    @JvmStatic fun nativeSignInAgainBodyToJsonFallback(paramsJson: String): String { return "" }

    @JvmStatic fun nativeSpaceAddChildFallback(): String { return "" }

    @JvmStatic fun nativeSpaceAddChildRawFallback(): String { return "" }

    @JvmStatic fun nativeSpaceGetChildrenFallback(spaceId: String): String { return "" }

    @JvmStatic fun nativeSpaceGetDepthFallback(roomId: String): Int { return 0 }

    @JvmStatic fun nativeSpaceGetParentsFallback(roomId: String): String { return "" }

    @JvmStatic fun nativeSpaceIsInSpaceFallback(spaceId: String, roomId: String): Boolean { return false }

    @JvmStatic fun nativeSpaceResetFallback(): String { return "" }

    @JvmStatic fun nativeSpaceSearchFallback(spaceId: String, query: String): String { return "" }

    @JvmStatic fun nativeSpaceSetMetadataFallback(): String { return "" }

    @JvmStatic fun nativeSpaceSetRootFallback(): String { return "" }

    @JvmStatic fun nativeSpaceToTreeFallback(spaceId: String, maxDepth: Int): String { return "" }

    @JvmStatic fun nativeSpaceTraverseFallback(mode: Int, maxDepth: Int): String { return "" }

    @JvmStatic fun nativeSpoilerBuildContentFallback(body: String, mxcUrl: String, msgType: String, reason: String): String { return "" }

    @JvmStatic fun nativeSpoilerBuildImageFallback(body: String, mxcUrl: String, mimeType: String, width: Int, height: Int, sizeBytes: Long, reason: String): String { return "" }

    @JvmStatic fun nativeSpoilerBuildTextFallback(body: String, reason: String): String { return "" }

    @JvmStatic fun nativeSpoilerDetectTypeFallback(formattedBody: String): String { return "" }

    @JvmStatic fun nativeSpoilerHasSpoilerFallback(formattedBody: String): Boolean { return false }

    @JvmStatic fun nativeSqliteDbBeginTransactionFallback(): String { return "" }

    @JvmStatic fun nativeSqliteDbCloseFallback(): String { return "" }

    @JvmStatic fun nativeSqliteDbCommitTransactionFallback(): Int { return 0 }

    @JvmStatic fun nativeSqliteDbCountEventsFallback(key: String, roomId: String): Int { return 0 }

    @JvmStatic fun nativeSqliteDbDeleteEventFallback(): Int { return 0 }

    @JvmStatic fun nativeSqliteDbInsertEventFallback(key: String, eventId: String, roomId: String, type: String, senderId: String, contentJson: String, originTs: Long, ageTs: Long, displayIndex: Int): Boolean { return false }

    @JvmStatic fun nativeSqliteDbInsertEventRelFallback(key: String, eventId: String, roomId: String, type: String, senderId: String, contentJson: String, originTs: Long, ageTs: Long, displayIndex: Int, stateKey: String, redacts: String, relType: String, relatesToId: String): Boolean { return false }

    @JvmStatic fun nativeSqliteDbMaxDisplayIndexFallback(key: String, roomId: String): Int { return 0 }

    @JvmStatic fun nativeSqliteDbOpenFallback(dbPath: String, key: String): Boolean { return false }

    @JvmStatic fun nativeSqliteDbQueryEventFallback(key: String, eventId: String): String { return "" }

    @JvmStatic fun nativeSqliteDbQueryEventsFallback(key: String, roomId: String, limit: Int, offset: Int, ascending: Boolean): String { return "" }

    @JvmStatic fun nativeSqliteDbQueryRoomsFallback(key: String): String { return "" }

    @JvmStatic fun nativeSqliteDbSchemaVersionFallback(key: String): Int { return 0 }

    @JvmStatic fun nativeSqliteDbUpsertRoomFallback(key: String, roomId: String, displayName: String, avatarUrl: String, topic: String, membership: String, notifCount: Int, highlightCount: Int, lastActivityMs: Long, isDirect: Boolean, isSpace: Boolean, isFavourite: Boolean, isEncrypted: Boolean): Boolean { return false }

    @JvmStatic fun nativeStripHtmlTagsFallback(html: String): String { return "" }

    @JvmStatic fun nativeSuggestAliasesFallback(roomName: String): String { return "" }

    @JvmStatic fun nativeSuggestChunkSizeMbFallback(fileSize: Long): Int { return 0 }

    @JvmStatic fun nativeSyncResponseRoundtripFallback(json: String): String { return "" }

    @JvmStatic fun nativeTermsAreRequiredFallback(errorJson: String): Boolean { return false }

    @JvmStatic fun nativeTermsBuildAgreeFallback(urlsJson: String): String { return "" }

    @JvmStatic fun nativeTermsGetPendingFallback(responseJson: String, agreedJson: String): String { return "" }

    @JvmStatic fun nativeTermsParseFallback(json: String): String { return "" }

    @JvmStatic fun nativeThreadAddReplyFallback(): String { return "" }

    @JvmStatic fun nativeThreadExtractRootFallback(eventContent: String): String { return "" }

    @JvmStatic fun nativeThreadFormatCountFallback(count: Int): String { return "" }

    @JvmStatic fun nativeThreadGetListFallback(limit: Int, offset: Int): String { return "" }

    @JvmStatic fun nativeThreadGetNotificationsFallback(): String { return "" }

    @JvmStatic fun nativeThreadGetUnreadStateFallback(threadId: String): String { return "" }

    @JvmStatic fun nativeThreadIsRootFallback(eventContent: String, eventId: String): Boolean { return false }

    @JvmStatic fun nativeThreadMarkReadFallback(): String { return "" }

    @JvmStatic fun nativeThreadResetFallback(): String { return "" }

    @JvmStatic fun nativeThreadSetUnreadFallback(): String { return "" }

    @JvmStatic fun nativeThreadTotalUnreadFallback(): Int { return 0 }

    @JvmStatic fun nativeThreadUpsertFallback(): String { return "" }

    @JvmStatic fun nativeTlsBridgeAvailableFallback(): Boolean { return false }

    @JvmStatic fun nativeTrackPollResponseFallback(optionId: String, userId: String): String { return "" }

    @JvmStatic fun nativeTruncateDescriptionFallback(text: String, maxLen: Int): String { return "" }

    @JvmStatic fun nativeTruncateReportDescriptionFallback(description: String, maxLen: Int): String { return "" }

    @JvmStatic fun nativeUndoCheckpointFallback(): String { return "" }

    @JvmStatic fun nativeUndoDoFallback(): String { return "" }

    @JvmStatic fun nativeUndoGetStateFallback(): String { return "" }

    @JvmStatic fun nativeUndoOnBeforePasteFallback(): String { return "" }

    @JvmStatic fun nativeUndoOnSelectAllFallback(): String { return "" }

    @JvmStatic fun nativeUndoRedoFallback(): String { return "" }

    @JvmStatic fun nativeUndoSetConfigFallback(): String { return "" }

    @JvmStatic fun nativeUnpinEventFallback(roomId: String, eventId: String, removedBy: String, powerLevel: Int): String { return "" }

    @JvmStatic fun nativeUploadBuildContentFallback(attachmentJson: String, mxcUrl: String): String { return "" }

    @JvmStatic fun nativeUploadFormatSizeWarningFallback(fileSize: Long, maxSize: Long): String { return "" }

    @JvmStatic fun nativeUploadGetProgressFallback(): String { return "" }

    @JvmStatic fun nativeUploadIsSizeValidFallback(fileSize: Long): Boolean { return false }

    @JvmStatic fun nativeUploadParseResponseFallback(json: String): String { return "" }

    @JvmStatic fun nativeUploadResetProgressFallback(): String { return "" }

    @JvmStatic fun nativeUploadSetMaxSizeFallback(): String { return "" }

    @JvmStatic fun nativeUploaderAdvanceFallback(): String { return "" }

    @JvmStatic fun nativeUploaderCancelFallback(): String { return "" }

    @JvmStatic fun nativeUploaderComputeChunksFallback(fileSize: Long): Int { return 0 }

    @JvmStatic fun nativeUploaderContentRangeFallback(index: Int): String { return "" }

    @JvmStatic fun nativeUploaderGetChunkInfoFallback(index: Int): String { return "" }

    @JvmStatic fun nativeUploaderProgressFallback(): String { return "" }

    @JvmStatic fun nativeUploaderResetFallback(): String { return "" }

    @JvmStatic fun nativeUploaderSetChunkSizeMbFallback(): Int { return 0 }

    @JvmStatic fun nativeUrlPreviewToJsonFallback(previewJson: String): String { return "" }

    @JvmStatic fun nativeUserDirAvatarInitFallback(displayName: String, userId: String): String { return "" }

    @JvmStatic fun nativeUserDirBestNameFallback(displayName: String, userId: String): String { return "" }

    @JvmStatic fun nativeUserDirBuildSearchFallback(searchTerm: String, limit: Int): String { return "" }

    @JvmStatic fun nativeUserDirIsValidQueryFallback(query: String): Boolean { return false }

    @JvmStatic fun nativeUserDirSearchFallback(query: String, responseJson: String): String { return "" }

    @JvmStatic fun nativeUserIdToDisplayNameFallback(userId: String, capitalize: Boolean): String { return "" }

    @JvmStatic fun nativeValidateAndFormatRecoveryKeyFallback(rawKey: String): String { return "" }

    @JvmStatic fun nativeValidatePasswordFallback(password: String): String { return "" }

    @JvmStatic fun nativeValidateRecoveryKeyFallback(key: String): Boolean { return false }

    @JvmStatic fun nativeValidateWidgetSecurityFallback(url: String, policyJson: String): String { return "" }

    @JvmStatic fun nativeVerifyDeviceSignatureFallback(deviceKeysJson: String, userId: String, deviceId: String, signKeyB64: String, signatureB64: String): Boolean { return false }

    @JvmStatic fun nativeVerifyEventSignatureFallback(eventJson: String, signKeyB64: String): Boolean { return false }

    @JvmStatic fun nativeWidgetMgrApproveCapabilityFallback(widgetId: String, capability: Int): String { return "" }

    @JvmStatic fun nativeWidgetMgrBuildCspFallback(): String { return "" }

    @JvmStatic fun nativeWidgetMgrBuildPostMessageFallback(widgetId: String, action: String, data: String): String { return "" }

    @JvmStatic fun nativeWidgetMgrCountFallback(): String { return "" }

    @JvmStatic fun nativeWidgetMgrCreateWidgetFallback(widgetId: String, type: String, url: String, name: String, waitForIframeLoad: Boolean): String { return "" }

    @JvmStatic fun nativeWidgetMgrDenyCapabilityFallback(widgetId: String, capability: Int): String { return "" }

    @JvmStatic fun nativeWidgetMgrGetByTypeFallback(type: String): String { return "" }

    @JvmStatic fun nativeWidgetMgrGetUrlFallback(widgetId: String): String { return "" }

    @JvmStatic fun nativeWidgetMgrInitFallback(roomId: String, userId: String, displayName: String, avatarUrl: String): Boolean { return false }

    @JvmStatic fun nativeWidgetMgrLoadWidgetsFallback(stateEventsJson: String): String { return "" }

    @JvmStatic fun nativeWidgetMgrParsePostMessageFallback(message: String): String { return "" }

    @JvmStatic fun nativeWidgetMgrRemoveWidgetFallback(widgetId: String): String { return "" }

    @JvmStatic fun nativeWidgetMgrRequestCapabilityFallback(widgetId: String, capability: Int): String { return "" }

    @JvmStatic fun nativeWidgetMgrResizeFallback(widgetId: String, width: Int, height: Int): String { return "" }

    @JvmStatic fun nativeWidgetMgrSetMaximizedFallback(widgetId: String, maximized: Boolean): String { return "" }

    @JvmStatic fun nativeWidgetMgrSetMinimizedFallback(widgetId: String, minimized: Boolean): String { return "" }

    @JvmStatic fun nativeWidgetMgrSetPinnedFallback(widgetId: String, pinned: Boolean): String { return "" }

    @JvmStatic fun nativeWidgetMgrSetSecurityPolicyFallback(policyJson: String): Boolean { return false }

    @JvmStatic fun nativeWidgetMgrSupportsPiPFallback(widgetId: String): Boolean { return false }

    @JvmStatic fun parseDirectMessagesFallback(json: String): String { return "" }

    @JvmStatic fun parseIgnoredUsersFallback(json: String): String { return "" }

    @JvmStatic fun parseOAuthCallbackFallback(url: String, redirectUri: String): String { return "" }

    @JvmStatic fun parseRelationFallback(eventJson: String, allowedTypes: String): JSONObject { return JSONObject() }

    @JvmStatic fun parseResponseFallback(responseBody: String?, httpStatus: Int): JSONObject { return JSONObject() }

    @JvmStatic fun parseSearchResponseFallback(engine: String, json: String, query: String): String { return "" }

    @JvmStatic fun parseSlashCommandFallback(text: String): JSONObject { return JSONObject() }

    @JvmStatic fun parseTranslateResponseFallback(responseBody: String?, httpStatus: Int): JSONObject { return JSONObject() }

    @JvmStatic fun parseWebCommandFallback(args: String): String { return "" }

    @JvmStatic fun prioritizeServersFallback(serversJson: String): String { return "" }

    @JvmStatic fun sanitizeFilenameFallback(name: String, maxLen: Int): String { return "" }

    @JvmStatic fun serverNameFromMxidFallback(mxid: String): String { return "" }

    @JvmStatic fun shouldAutoDraftFallback(text: String, threshold: Int): Boolean { return false }

    @JvmStatic fun shouldShowJumpToUnreadFallback(readMarkerJson: String): Boolean { return false }

    @JvmStatic fun swapAccountOrderFallback(accountsJson: String, posA: Int, posB: Int): String { return "" }

    @JvmStatic fun timelineAddEventsFallback(roomId: String, eventsJson: String, prevToken: String, nextToken: String, direction: Int): Int { return 0 }

    @JvmStatic fun timelineAddSyncEventFallback(roomId: String, eventId: String, type: String, senderId: String, contentJson: String, originTs: Long, displayIndex: Int, stateKey: String, redacts: String, relType: String, relatesToId: String): Int { return 0 }

    @JvmStatic fun timelineAttachDbFallback(roomId: String, dbKey: String): Boolean { return false }

    @JvmStatic fun timelineChunkCountFallback(roomId: String): Int { return 0 }

    @JvmStatic fun timelineClearFallback(): String { return "" }

    @JvmStatic fun timelineEventsAvailableFallback(roomId: String, direction: Int): Int { return 0 }

    @JvmStatic fun timelineGetEventFallback(eventId: String): String { return "" }

    @JvmStatic fun timelineGetEventsFallback(roomId: String): String { return "" }

    @JvmStatic fun timelineGetLatestEditFallback(eventId: String): String { return "" }

    @JvmStatic fun timelineGetRepliesFallback(eventId: String): String { return "" }

    @JvmStatic fun timelineGetSnapshotFallback(roomId: String, limit: Int, offset: Int): String { return "" }

    @JvmStatic fun timelineGetThreadEventsFallback(rootEventId: String): String { return "" }

    @JvmStatic fun tlsRequestFallback(host: String, port: Int, request: String, timeoutMs: Int): String { return "" }

    @JvmStatic fun validateAndBuildFallback(roomId: String, dateString: String, serverUrl: String, accessToken: String, isEnabled: Boolean): JSONObject { return JSONObject() }

    @JvmStatic fun wrapWithRelationFallback(contentJson: String, relationJson: String): String { return "" }
}
