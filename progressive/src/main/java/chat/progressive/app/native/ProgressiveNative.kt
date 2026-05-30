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
    @JvmStatic external fun nativeConnMonitorOnReconnectAttempt()
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

    // ---- IDN (Internationalized Domain Names) ----
    @JvmStatic external fun nativeToPunycode(domain: String): String
    @JvmStatic external fun nativeFromPunycode(domain: String): String

}
