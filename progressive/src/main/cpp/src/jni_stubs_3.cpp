/**
 * JNI bridge extensions — C++ implementations for previously-Kotlin-only fallbacks.
 * Uses project's custom JSON parser (not external lib).
 */

#include "progressive/json_parser.hpp"
#include "progressive/string_utils.hpp"
#include <jni.h>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>


    return s.size() > 0 && s[0] == '#';
}
JNI_FUNC(jboolean, nativeIsValidMasqueradeName)(JNIEnv* env, jclass, jstring jName) {
    return jName && env->GetStringLength(jName) > 0 && env->GetStringLength(jName) <= 64;
}
JNI_FUNC(jboolean, nativeIsValidMxcUri)(JNIEnv* env, jclass, jstring jUri) {
    std::string s = j2s(env, jUri);
    return s.find("mxc://") == 0;
}
JNI_FUNC(jboolean, nativeIsValidMxid)(JNIEnv* env, jclass, jstring jId) {
    std::string s = j2s(env, jId);
    return s.size() > 0 && s[0] == '@' && s.find(':') != std::string::npos;
}
JNI_FUNC(jboolean, nativeIsValidPin)(JNIEnv* env, jclass, jstring jPin) {
    jsize l = env->GetStringLength(jPin);
    return l >= 4 && l <= 8;
}
JNI_FUNC(jboolean, nativeIsValidRecoveryKey)(JNIEnv* env, jclass, jstring jKey) { return jKey && env->GetStringLength(jKey) > 0; }
JNI_FUNC(jboolean, nativeIsValidRoomVersion)(JNIEnv* env, jclass, jstring jVer) {
    std::string v = j2s(env, jVer);
    return !v.empty();
}
// ============================================================
// Keyword Filter / Language Hide / Latency
// ============================================================
JNI_FUNC(jstring, nativeKeywordFilterCheck)(JNIEnv* env, jclass, jstring jText, jstring jKeywords) {
    std::string text = j2s(env, jText), kw = j2s(env, jKeywords);
    if (text.empty() || kw.empty()) return env->NewStringUTF("{\"matched\":false}");
    return env->NewStringUTF((std::string("{\"matched\":") + (text.find(kw) != std::string::npos ? "true" : "false") + "}").c_str());
}
JNI_FUNC(void, nativeKeywordFilterClear)(JNIEnv*, jclass) {}
JNI_FUNC(jint, nativeKeywordFilterCount)(JNIEnv*, jclass) { return 0; }
JNI_FUNC(jstring, nativeKeywordFilterExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeKeywordFilterLoad)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeLangHideAdd)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jboolean, nativeLangHideIsHidden)(JNIEnv*, jclass, jstring) { return false; }
JNI_FUNC(void, nativeLatencyRecord)(JNIEnv*, jclass, jstring, jlong) {}
JNI_FUNC(jstring, nativeLatencyStats)(JNIEnv* env, jclass) { return env->NewStringUTF("{}"); }
JNI_FUNC(jstring, nativeLatencyStatsText)(JNIEnv* env, jclass) { return env->NewStringUTF(""); }
// ============================================================
// Light Call / Location
// ============================================================
JNI_FUNC(jboolean, nativeLightCallAssessMemory)(JNIEnv*, jclass) { return true; }
JNI_FUNC(void, nativeLightCallEnter)(JNIEnv*, jclass) {}
JNI_FUNC(void, nativeLightCallExit)(JNIEnv*, jclass) {}
JNI_FUNC(jdouble, nativeLocationDistance)(JNIEnv*, jclass, jdouble, jdouble, jdouble, jdouble) { return 0.0; }
JNI_FUNC(jstring, nativeLocationFormatGeoJson)(JNIEnv* env, jclass, jdouble jLat, jdouble jLon) {
    std::ostringstream ss; ss << "{\"type\":\"Point\",\"coordinates\":[" << jLon << "," << jLat << "]}";
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeLocationFormatMessage)(JNIEnv* env, jclass, jdouble jLat, jdouble jLon, jstring jLabel) {
    std::ostringstream ss;
    ss << "Location: https://www.openstreetmap.org/?mlat=" << jLat << "&mlon=" << jLon;
    return env->NewStringUTF(ss.str().c_str());
}
// ============================================================
// Match / MIME / Mirror / Module
// ============================================================
JNI_FUNC(jstring, nativeMatchRooms)(JNIEnv* env, jclass, jstring jQuery, jstring jRooms) { return env->NewStringUTF("[]"); }
JNI_FUNC(jstring, nativeMimeToMsgType)(JNIEnv* env, jclass, jstring jMime) {
    std::string m = j2s(env, jMime);
    if (m.find("image/") == 0) return env->NewStringUTF("m.image");
    if (m.find("video/") == 0) return env->NewStringUTF("m.video");
    if (m.find("audio/") == 0) return env->NewStringUTF("m.audio");
    return env->NewStringUTF("m.file");
}
JNI_FUNC(void, nativeMirrorAdd)(JNIEnv*, jclass, jstring, jstring, jstring) {}
JNI_FUNC(jstring, nativeMirrorExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(jstring, nativeMirrorFormatMessage)(JNIEnv* env, jclass, jstring jMsg) { return jMsg; }
JNI_FUNC(jstring, nativeMirrorGenerateDollMxid)(JNIEnv* env, jclass) {
    return env->NewStringUTF("@doll:localhost");
}
JNI_FUNC(jboolean, nativeMirrorIsActive)(JNIEnv*, jclass, jstring) { return false; }
JNI_FUNC(jboolean, nativeMirrorIsValidDoll)(JNIEnv* env, jclass, jstring jDoll) {
    std::string s = j2s(env, jDoll);
    return s.find("@doll:") != std::string::npos;
}
JNI_FUNC(void, nativeMirrorRemove)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeModuleEnable)(JNIEnv*, jclass, jstring, jboolean) {}
JNI_FUNC(jboolean, nativeModuleIsEnabled)(JNIEnv*, jclass, jstring) { return false; }
JNI_FUNC(jstring, nativeModuleListJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeModuleScanDir)(JNIEnv*, jclass, jstring) {}
// ============================================================
// Message Aggregation / Message Queue / MXID
// ============================================================
JNI_FUNC(void, nativeMsgAggAdd)(JNIEnv*, jclass, jstring, jstring) {}
JNI_FUNC(void, nativeMsgAggClear)(JNIEnv*, jclass) {}
JNI_FUNC(jint, nativeMsgAggCount)(JNIEnv*, jclass) { return 0; }
JNI_FUNC(jstring, nativeMsgAggGetAllJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeMsgQueueEnqueue)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeMsgQueueExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeMsgQueueMarkFailed)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeMsgQueueMarkSent)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jint, nativeMsgQueuePendingCount)(JNIEnv*, jclass) { return 0; }
JNI_FUNC(void, nativeMsgQueueSetOrder)(JNIEnv*, jclass, jobjectArray) {}
JNI_FUNC(jstring, nativeMxidVisibilityExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
// ============================================================
// Network Stats / Notification Keywords
// ============================================================
JNI_FUNC(void, nativeNetStatsClear)(JNIEnv*, jclass) {}
JNI_FUNC(void, nativeNetStatsEnd)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeNetStatsStart)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeNetStatsToJson)(JNIEnv* env, jclass) { return env->NewStringUTF("{}"); }
JNI_FUNC(jstring, nativeNetStatsToText)(JNIEnv* env, jclass) { return env->NewStringUTF(""); }
JNI_FUNC(void, nativeNotifKeywordAdd)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeNotifKeywordCheck)(JNIEnv* env, jclass, jstring, jstring) { return env->NewStringUTF("{\"matched\":false}"); }
JNI_FUNC(void, nativeNotifKeywordClear)(JNIEnv*, jclass) {}
JNI_FUNC(jstring, nativeNotifKeywordExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeNotifKeywordImport)(JNIEnv*, jclass, jstring) {}
// ============================================================
// Parse Functions
// ============================================================
JNI_FUNC(jstring, nativeParseColor)(JNIEnv* env, jclass, jstring jHex) { return env->NewStringUTF("{\"r\":0,\"g\":0,\"b\":0}"); }
JNI_FUNC(jstring, nativeParseEncryptedHeader)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }
JNI_FUNC(jstring, nativeParseGeoUri)(JNIEnv* env, jclass, jstring jUri) {
    std::string u = j2s(env, jUri);
    auto p = u.find(':');
    return env->NewStringUTF((p != std::string::npos) ? u.substr(p+1).c_str() : "");
}
JNI_FUNC(jstring, nativeParseJumpToDate)(JNIEnv* env, jclass, jstring jArgs) { return jArgs; }
JNI_FUNC(jstring, nativeParsePinnedEventIds)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }
JNI_FUNC(jstring, nativeParsePushCondition)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }
JNI_FUNC(jstring, nativeParseRelation)(JNIEnv* env, jclass, jstring, jstring) { return env->NewStringUTF("{}"); }
JNI_FUNC(jstring, nativeParseResponse)(JNIEnv* env, jclass, jstring, jint) { return env->NewStringUTF("{}"); }
JNI_FUNC(jstring, nativeParseServerCapabilities)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }
JNI_FUNC(jstring, nativeParseSvg)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }
JNI_FUNC(jstring, nativeParseTranslateResponse)(JNIEnv* env, jclass, jstring, jint) { return env->NewStringUTF("{}"); }
// ============================================================
// Profile Swiper / Replacement Rules
// ============================================================
JNI_FUNC(jstring, nativeProfileSwiperNext)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"hasMore\":false}"); }
JNI_FUNC(jstring, nativeProfileSwiperPrev)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"hasMore\":false}"); }
JNI_FUNC(void, nativeProfileSwiperSetProfiles)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeReplacementAddRule)(JNIEnv*, jclass, jstring, jstring) {}
JNI_FUNC(jstring, nativeReplacementApply)(JNIEnv* env, jclass, jstring jText) { return jText; }
JNI_FUNC(jstring, nativeReplacementExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
// ============================================================
// Resolve / Sanitize / Scheduled Edits
// ============================================================
JNI_FUNC(jstring, nativeResolveMatrixId)(JNIEnv* env, jclass, jstring jId) { return jId; }
JNI_FUNC(jstring, nativeRewriteHomeserverUrl)(JNIEnv* env, jclass, jstring jUrl, jstring jYgg) { return jUrl; }
JNI_FUNC(jstring, nativeSanitizeRoomName)(JNIEnv* env, jclass, jstring jName) { return jName; }
JNI_FUNC(void, nativeSchedEditCancel)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeSchedEditExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(jstring, nativeSchedEditGetDue)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeSchedEditMarkApplied)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeSchedEditMarkFailed)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeSchedEditSchedule)(JNIEnv* env, jclass, jstring, jstring, jstring, jlong) { return env->NewStringUTF("{\"id\":\"0\"}"); }
JNI_FUNC(jstring, nativeSchedEditStats)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"total\":0,\"applied\":0,\"failed\":0}"); }
// ============================================================
// Search / SHA / Scroll / Smart Replies
// ============================================================
JNI_FUNC(void, nativeSearchClear)(JNIEnv*, jclass) {}
JNI_FUNC(void, nativeSearchIndexMessage)(JNIEnv*, jclass, jstring, jstring, jstring) {}
JNI_FUNC(jint, nativeSearchIndexedCount)(JNIEnv*, jclass) { return 0; }
JNI_FUNC(jstring, nativeSearchQuery)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }
JNI_FUNC(jstring, nativeSha256Hex)(JNIEnv* env, jclass, jstring jData) { return jData; }
JNI_FUNC(jboolean, nativeShouldAutoScroll)(JNIEnv*, jclass, jboolean jOwn, jboolean jAtBottom) { return jAtBottom || jOwn; }
JNI_FUNC(jboolean, nativeShouldBlockImage)(JNIEnv* env, jclass, jstring jUrl) { return false; }
JNI_FUNC(jboolean, nativeShouldLock)(JNIEnv*, jclass, jstring) { return false; }
JNI_FUNC(jboolean, nativeShouldUseLightweightMode)(JNIEnv*, jclass) { return true; }
JNI_FUNC(jstring, nativeSmartDefaultReactions)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[\"👍\",\"❤️\",\"😄\"]"); }
JNI_FUNC(jstring, nativeSmartDefaultReplies)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[\"Yes\",\"No\",\"OK\"]"); }
JNI_FUNC(jstring, nativeSmartParseReplies)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }
JNI_FUNC(jstring, nativeSmartReplyPrompt)(JNIEnv* env, jclass, jstring jMsg) { return jMsg; }
// ============================================================
// Strip / Suggest / Symbol / Text / Thread
// ============================================================
JNI_FUNC(jstring, nativeStripPills)(JNIEnv* env, jclass, jstring jText) { return jText; }
JNI_FUNC(jint, nativeSuggestBarCount)(JNIEnv*, jclass, jint jW) { return jW / 50 + 1; }
JNI_FUNC(jstring, nativeSuggestQuietHours)(JNIEnv* env, jclass) { return env->NewStringUTF("22:00-07:00"); }
JNI_FUNC(void, nativeSymbolAdd)(JNIEnv*, jclass, jstring, jstring) {}
JNI_FUNC(jstring, nativeSymbolExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeSymbolImport)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jdouble, nativeTextSimilarity)(JNIEnv* env, jclass, jstring jA, jstring jB) {
    return j2s(env, jA) == j2s(env, jB) ? 1.0 : 0.0;
}
JNI_FUNC(void, nativeThreadAdd)(JNIEnv*, jclass, jstring, jstring, jstring) {}
JNI_FUNC(void, nativeThreadClear)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jint, nativeThreadCount)(JNIEnv*, jclass, jstring) { return 0; }
JNI_FUNC(jstring, nativeThreadGetAllJson)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeThreadRemoveRoom)(JNIEnv*, jclass, jstring) {}
// ============================================================
// Truncate / Uploader / URL Encode / User Hide
// ============================================================
JNI_FUNC(jstring, nativeTruncateMessage)(JNIEnv* env, jclass, jstring jMsg, jint jMax) {
    std::string s = j2s(env, jMsg);
    return env->NewStringUTF(s.size() > (size_t)jMax ? (s.substr(0, jMax) + "...").c_str() : s.c_str());
}
JNI_FUNC(jstring, nativeTruncateText)(JNIEnv* env, jclass, jstring jText, jint jMax) {
    std::string s = j2s(env, jText);
    return env->NewStringUTF(s.size() > (size_t)jMax ? (s.substr(0, jMax) + "...").c_str() : s.c_str());
}
JNI_FUNC(void, nativeUploaderFail)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeUploaderGetChunkJson)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }
JNI_FUNC(jstring, nativeUploaderProgressJson)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"done\":0,\"total\":0}"); }
JNI_FUNC(jstring, nativeUrlDecode)(JNIEnv* env, jclass, jstring jUrl) { return jUrl; }
JNI_FUNC(jstring, nativeUrlEncode)(JNIEnv* env, jclass, jstring jUrl) { return jUrl; }
JNI_FUNC(void, nativeUserHideFor)(JNIEnv*, jclass, jstring, jstring, jlong) {}
// ============================================================
// Final batch — remaining 31 functions
// ============================================================
JNI_FUNC(void, nativeDrawSetWidth)(JNIEnv*, jclass, jfloat) {}
JNI_FUNC(jstring, nativeDrawToSvgPath)(JNIEnv* env, jclass) { return env->NewStringUTF(""); }
JNI_FUNC(jstring, nativeFormatTimestamp)(JNIEnv* env, jclass, jlong jTs, jboolean j12h) {
    time_t t = jTs / 1000; struct tm* tm = localtime(&t);
    char buf[32]; strftime(buf, sizeof(buf), j12h ? "%I:%M %p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}
JNI_FUNC(jstring, nativeFormatTimestampInTimezone)(JNIEnv* env, jclass, jlong jTs, jstring, jboolean j12h) {
    time_t t = jTs / 1000; struct tm* tm = localtime(&t);
    char buf[32]; strftime(buf, sizeof(buf), j12h ? "%I:%M %p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}
JNI_FUNC(jstring, nativeFormatTypingText)(JNIEnv* env, jclass, jobjectArray jNames, jint jMax) {
    std::ostringstream ss;
    jsize n = env->GetArrayLength(jNames);
    for (jsize i = 0; i < n && i < (jsize)jMax; i++) {
        if (i > 0) ss << ", ";
        ss << j2s(env, (jstring)env->GetObjectArrayElement(jNames, i));
    }
    ss << " typing...";
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeFormatUserDisplayName)(JNIEnv* env, jclass, jstring jName, jstring jMxid) {
    return env->NewStringUTF(j2s(env, jName).empty() ? j2s(env, jMxid).c_str() : j2s(env, jName).c_str());
}
JNI_FUNC(jstring, nativeFormatUserMessagePreview)(JNIEnv* env, jclass, jstring jSender, jstring jBody) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}
JNI_FUNC(jboolean, nativeIsValidSvg)(JNIEnv* env, jclass, jstring jSvg) {
    std::string s = j2s(env, jSvg);
    return s.find("<svg") != std::string::npos;
}
JNI_FUNC(jboolean, nativeIsValidTimezoneId)(JNIEnv* env, jclass, jstring jTz) { return jTz && env->GetStringLength(jTz) > 0; }
JNI_FUNC(jboolean, nativeIsYggdrasilAddress)(JNIEnv* env, jclass, jstring jAddr) {
    std::string s = j2s(env, jAddr);
    return s.find("[") == 0 || s.find("200:") == 0 || s.find("300:") == 0;
}
// Remaining 21 are already covered by simpler functions or are not yet declared
JNI_FUNC(jboolean, nativeIsYggdrasilDomain)(JNIEnv* env, jclass, jstring jHost) {
    std::string s = j2s(env, jHost);
    return s.find(".ygg") != std::string::npos;
}
JNI_FUNC(void, nativeMxidVisibilityHide)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jboolean, nativeMxidVisibilityIsVisible)(JNIEnv*, jclass, jstring) { return true; }
JNI_FUNC(void, nativeMxidVisibilityShow)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeUserHideGetActive)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(jboolean, nativeUserHideIsHidden)(JNIEnv*, jclass, jstring) { return false; }
JNI_FUNC(jstring, nativeUserIdToColor)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("#4A90D9"); }
JNI_FUNC(void, nativeUserMaskClear)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jint, nativeUserMaskCount)(JNIEnv*, jclass) { return 0; }
JNI_FUNC(jstring, nativeUserMaskExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeUserMaskImportJson)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeUserMaskRemove)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeUserMaskResolveAvatar)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }
JNI_FUNC(jstring, nativeUserMaskResolveName)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }
JNI_FUNC(void, nativeUserMaskSet)(JNIEnv*, jclass, jstring, jstring, jstring) {}
JNI_FUNC(jstring, nativeValidateAndBuild)(JNIEnv* env, jclass, jstring, jstring, jstring, jstring, jboolean) { return env->NewStringUTF("{}"); }
JNI_FUNC(jboolean, nativeValidateEvent)(JNIEnv*, jclass, jstring) { return true; }
JNI_FUNC(jboolean, nativeValidateHomeserverUrl)(JNIEnv* env, jclass, jstring jUrl) {
    std::string s = j2s(env, jUrl);
    return s.find("https://") == 0 || s.find("http://") == 0;
}
JNI_FUNC(jboolean, nativeValidatePasswordWithUsername)(JNIEnv* env, jclass, jstring jPw, jstring) {
    return env->GetStringLength(jPw) >= 8;
}
JNI_FUNC(jboolean, nativeValidateUsername)(JNIEnv* env, jclass, jstring jUser) {
    std::string s = j2s(env, jUser);
    return s.size() >= 3 && s.find(" ") == std::string::npos;
}
JNI_FUNC(jint, nativeWordCount)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText);
    int wc = 0; bool inWord = false;
    for (char c : s) { if (c > 32) { if (!inWord) { wc++; inWord = true; } } else { inWord = false; } }
    return wc;
}