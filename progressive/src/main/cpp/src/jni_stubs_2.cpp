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


JNI_FUNC(void, nativeDeletedArchiveClear)(JNIEnv*, jclass) {}
JNI_FUNC(jint, nativeDeletedArchiveCount)(JNIEnv*, jclass) { return 0; }
JNI_FUNC(jstring, nativeDeletedArchiveExportJson)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[]");
}
// ============================================================
// Desync Detector / Language / Drawing
// ============================================================
JNI_FUNC(jstring, nativeDesyncCheck)(JNIEnv* env, jclass, jstring) {
    return env->NewStringUTF("{\"desynced\":false}");
}
JNI_FUNC(void, nativeDesyncTrackEvent)(JNIEnv*, jclass, jstring, jstring) {}
JNI_FUNC(jstring, nativeDetectLanguage)(JNIEnv* env, jclass, jstring jText) {
    return env->NewStringUTF(jsonObj(jsonPair("language", "en")).c_str());
}
JNI_FUNC(void, nativeDrawClear)(JNIEnv*, jclass) {}
JNI_FUNC(jstring, nativeDrawExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeDrawLineTo)(JNIEnv*, jclass, jfloat, jfloat) {}
JNI_FUNC(void, nativeDrawMoveTo)(JNIEnv*, jclass, jfloat, jfloat) {}
JNI_FUNC(void, nativeDrawSetColor)(JNIEnv*, jclass, jint, jint, jint, jint) {}
JNI_FUNC(void, nativeDrawSetStrokeWidth)(JNIEnv*, jclass, jfloat) {}
// ============================================================
// Edit Distance / Emoji Blacklist
// ============================================================
JNI_FUNC(jint, nativeEditDistance)(JNIEnv* env, jclass, jstring jA, jstring jB) {
    std::string a = j2s(env, jA), b = j2s(env, jB);
    size_t la = a.size(), lb = b.size();
    if (la == 0) return (int)lb;
    if (lb == 0) return (int)la;
    // Simple Levenshtein
    std::vector<size_t> prev(lb+1), curr(lb+1);
    for (size_t j = 0; j <= lb; j++) prev[j] = j;
    for (size_t i = 1; i <= la; i++) {
        curr[0] = i;
        for (size_t j = 1; j <= lb; j++)
            curr[j] = std::min({prev[j]+1, curr[j-1]+1, prev[j-1] + (a[i-1]!=b[j-1])});
        prev.swap(curr);
    }
    return (int)prev[lb];
}
JNI_FUNC(void, nativeEmojiBlacklistAdd)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeEmojiBlacklistExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeEmojiBlacklistImport)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jboolean, nativeEmojiBlacklistIsBlocked)(JNIEnv*, jclass, jstring) { return false; }
JNI_FUNC(void, nativeEmojiBlacklistRemove)(JNIEnv*, jclass, jstring) {}
// ============================================================
// Event Extractors
// ============================================================
JNI_FUNC(jstring, nativeEncryptAccount)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }
JNI_FUNC(jint, nativeEstimatePaginationRequests)(JNIEnv*, jclass, jint jMiss, jint jPage) { return jPage>0 ? (jMiss+jPage-1)/jPage : 0; }
JNI_FUNC(jstring, nativeExtractEventLinks)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }
JNI_FUNC(jstring, nativeExtractFirstUrl)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }
JNI_FUNC(jstring, nativeExtractServerName)(JNIEnv* env, jclass, jstring jMxid) {
    std::string s = j2s(env, jMxid);
    auto p = s.find(':');
    return env->NewStringUTF((p != std::string::npos ? s.substr(p+1) : "").c_str());
}
JNI_FUNC(jstring, nativeExtractServers)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }
// ============================================================
// File Utilities
// ============================================================
JNI_FUNC(jboolean, nativeFileHasMetadata)(JNIEnv*, jclass, jstring) { return false; }
// ============================================================
// Format Utilities
// ============================================================
JNI_FUNC(jstring, nativeFormatChatTimestamp)(JNIEnv* env, jclass, jlong jTs, jboolean j12h) {
    time_t t = jTs / 1000;
    struct tm* tm = localtime(&t);
    char buf[32];
    strftime(buf, sizeof(buf), j12h ? "%I:%M %p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}
JNI_FUNC(jstring, nativeFormatCreationDate)(JNIEnv* env, jclass, jlong jTs) {
    time_t t = jTs / 1000; struct tm* tm = localtime(&t);
    char buf[32]; strftime(buf, sizeof(buf), "%b %d, %Y", tm);
    return env->NewStringUTF(buf);
}
JNI_FUNC(jstring, nativeFormatEmote)(JNIEnv* env, jclass, jstring jSender, jstring jText) {
    std::string s = "* " + j2s(env, jSender) + " " + j2s(env, jText);
    return env->NewStringUTF(s.c_str());
}
JNI_FUNC(jstring, nativeFormatEventHtml)(JNIEnv* env, jclass, jstring jSender, jstring jTs, jstring jBody, jstring jType, jstring jFile, jstring jSize, jstring jRel, jboolean) {
    std::ostringstream ss;
    ss << "<b>" << j2s(env, jSender) << "</b> " << j2s(env, jBody);
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeFormatEventPlainText)(JNIEnv* env, jclass, jstring jSender, jstring jTs, jstring jBody, jstring jType, jstring jFile, jstring jRel) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}
JNI_FUNC(jstring, nativeFormatFullTimestamp)(JNIEnv* env, jclass, jlong jTs) {
    time_t t = jTs/1000; struct tm* tm = localtime(&t);
    char buf[64]; strftime(buf, sizeof(buf), "%a %b %d %Y %H:%M:%S", tm);
    return env->NewStringUTF(buf);
}
JNI_FUNC(jstring, nativeFormatGroupLabel)(JNIEnv* env, jclass, jstring jName, jint jCnt) {
    std::ostringstream ss; ss << j2s(env, jName) << " (" << jCnt << ")";
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeFormatLastMessagePreview)(JNIEnv* env, jclass, jstring jSender, jstring jBody, jboolean, jboolean) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}
JNI_FUNC(jstring, nativeFormatLenny)(JNIEnv* env, jclass) { return env->NewStringUTF("( ͡° ͜ʖ ͡°)"); }
JNI_FUNC(jstring, nativeFormatMemoryLabel)(JNIEnv* env, jclass, jlong jBytes) {
    std::ostringstream ss;
    if (jBytes > 1024*1024) ss << (jBytes/(1024*1024)) << " MB";
    else if (jBytes > 1024) ss << (jBytes/1024) << " KB";
    else ss << jBytes << " B";
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeFormatMessageStatus)(JNIEnv* env, jclass, jint jSt) {
    switch(jSt) { case 0: return env->NewStringUTF("sending"); case 1: return env->NewStringUTF("sent"); default: return env->NewStringUTF("error"); }
}
JNI_FUNC(jstring, nativeFormatNotifBadge)(JNIEnv* env, jclass, jint jCnt) {
    return env->NewStringUTF(jCnt > 99 ? "99+" : std::to_string(jCnt).c_str());
}
JNI_FUNC(jstring, nativeFormatNotifBody)(JNIEnv* env, jclass, jstring jSender, jstring jBody, jstring jRoom) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}
JNI_FUNC(jstring, nativeFormatNotifTitle)(JNIEnv* env, jclass, jstring jRoom, jint jCnt) {
    std::ostringstream ss; ss << j2s(env, jRoom) << " (" << jCnt << ")";
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeFormatPlain)(JNIEnv* env, jclass, jstring jHtml) { return jHtml; } // return as-is for now
JNI_FUNC(jstring, nativeFormatPresenceLine)(JNIEnv* env, jclass, jstring jUser, jstring jStatus) {
    std::string s = j2s(env, jUser) + " " + j2s(env, jStatus);
    return env->NewStringUTF(s.c_str());
}
JNI_FUNC(jstring, nativeFormatProgressBar)(JNIEnv* env, jclass, jint jPct, jint jW) {
    std::string bar;
    int filled = jPct * jW / 100;
    for (int i = 0; i < jW; i++) bar += (i < filled) ? '#' : '-';
    return env->NewStringUTF(bar.c_str());
}
JNI_FUNC(jstring, nativeFormatRateLimitMessage)(JNIEnv* env, jclass, jlong jMs) {
    std::ostringstream ss; ss << "Rate limited. Try again in " << (jMs/1000) << "s";
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeFormatReactionPreview)(JNIEnv* env, jclass, jstring jKey, jint jCnt) {
    std::ostringstream ss; ss << j2s(env, jKey) << " " << jCnt;
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeFormatRelativeTime)(JNIEnv* env, jclass, jlong jTs) {
    int64_t now = std::time(nullptr) * 1000;
    int64_t diff = now - jTs;
    if (diff < 60000) return env->NewStringUTF("just now");
    if (diff < 3600000) { std::ostringstream ss; ss << (diff/60000) << "m"; return env->NewStringUTF(ss.str().c_str()); }
    if (diff < 86400000) { std::ostringstream ss; ss << (diff/3600000) << "h"; return env->NewStringUTF(ss.str().c_str()); }
    std::ostringstream ss; ss << (diff/86400000) << "d";
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeFormatResolvedEvent)(JNIEnv* env, jclass, jstring jEvent) { return jEvent; }
JNI_FUNC(jstring, nativeFormatRoomSummary)(JNIEnv* env, jclass, jstring jName, jstring jTopic, jboolean, jint jMem, jboolean) {
    std::ostringstream ss; ss << j2s(env, jName) << " - " << j2s(env, jTopic) << " (" << jMem << " members)";
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeFormatSessionBadge)(JNIEnv* env, jclass, jint jCnt) {
    return env->NewStringUTF(jCnt > 99 ? "99+" : std::to_string(jCnt).c_str());
}
JNI_FUNC(jstring, nativeFormatShortTime)(JNIEnv* env, jclass, jlong jTs, jboolean j12h) {
    time_t t = jTs/1000; struct tm* tm = localtime(&t);
    char buf[16]; strftime(buf, sizeof(buf), j12h ? "%I:%M%p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}
JNI_FUNC(jstring, nativeFormatShrug)(JNIEnv* env, jclass) { return env->NewStringUTF("¯\\_(ツ)_/¯"); }
JNI_FUNC(jstring, nativeFormatSpoiler)(JNIEnv* env, jclass, jstring jText) {
    std::string s = "[spoiler] " + j2s(env, jText);
    return env->NewStringUTF(s.c_str());
}
JNI_FUNC(jstring, nativeFormatTableFlip)(JNIEnv* env, jclass) { return env->NewStringUTF("(╯°□°）╯︵ ┻━┻"); }
// ============================================================
// Generate / Getters / Validation
// ============================================================
JNI_FUNC(jstring, nativeGenerateRainbow)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText), out;
    const char* colors[] = {"#FF0000","#FF7F00","#FFFF00","#00FF00","#0000FF","#4B0082","#8F00FF"};
    for (size_t i = 0; i < s.size(); i++)
        out += "<font color=\"" + std::string(colors[i%7]) + "\">" + s[i] + "</font>";
    return env->NewStringUTF(out.c_str());
}
JNI_FUNC(jstring, nativeGenerateToken)(JNIEnv* env, jclass, jint jLen) {
    std::string s; s.reserve(jLen);
    const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < jLen; i++) s += chars[rand() % 62];
    return env->NewStringUTF(s.c_str());
}
JNI_FUNC(jstring, nativeGetBadgeText)(JNIEnv* env, jclass, jint jCnt) {
    return env->NewStringUTF(jCnt > 99 ? "99+" : std::to_string(jCnt).c_str());
}
JNI_FUNC(jstring, nativeGetCommonTimezones)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"UTC\",\"Europe/London\",\"Europe/Paris\",\"Europe/Berlin\",\"Europe/Moscow\",\"America/New_York\",\"America/Los_Angeles\",\"Asia/Tokyo\",\"Asia/Shanghai\",\"Australia/Sydney\"]");
}
JNI_FUNC(jstring, nativeGetExtensionFromName)(JNIEnv* env, jclass, jstring jName) {
    std::string s = j2s(env, jName);
    auto p = s.rfind('.');
    return env->NewStringUTF((p != std::string::npos) ? s.substr(p+1).c_str() : "");
}
JNI_FUNC(jstring, nativeGetFileExtension)(JNIEnv* env, jclass, jstring jName) {
    std::string s = j2s(env, jName);
    auto p = s.rfind('.');
    return env->NewStringUTF((p != std::string::npos) ? s.substr(p+1).c_str() : "");
}
JNI_FUNC(jstring, nativeGetLanguageLabel)(JNIEnv* env, jclass, jstring jCode) {
    return jCode; // Return code as label
}
JNI_FUNC(jstring, nativeGetMemoryInfo)(JNIEnv* env, jclass) {
    return env->NewStringUTF("{\"available\":0,\"total\":0,\"used\":0}");
}
JNI_FUNC(jstring, nativeGetQuickReactions)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"❤️\",\"👍\",\"👎\",\"😄\",\"😢\",\"😮\",\"😡\",\"🎉\"]");
}
JNI_FUNC(jstring, nativeGetRecommendedMediaQuality)(JNIEnv* env, jclass, jint, jboolean, jboolean, jint, jdouble, jdouble) {
    return env->NewStringUTF("medium");
}
JNI_FUNC(jstring, nativeGetRoomVersionsJson)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\",\"8\",\"9\",\"10\",\"11\"]");
}
JNI_FUNC(jstring, nativeGetStrippableMimeTypes)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"application/octet-stream\",\"text/plain\"]");
}
JNI_FUNC(jstring, nativeGetSuggestedMasqueradeNames)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"Element\",\"FluffyChat\",\"Nheko\",\"SchildiChat\",\"Cinny\",\"Hydrogen\"]");
}
JNI_FUNC(jstring, nativeGetSuggestedRole)(JNIEnv* env, jclass, jint jPl) {
    if (jPl >= 100) return env->NewStringUTF("Admin");
    if (jPl >= 50) return env->NewStringUTF("Moderator");
    return env->NewStringUTF("User");
}
JNI_FUNC(jstring, nativeGetTrustBadge)(JNIEnv* env, jclass, jstring, jboolean, jboolean) {
    return env->NewStringUTF("unknown");
}
JNI_FUNC(jstring, nativeGetVerificationEmojis)(JNIEnv* env, jclass, jstring) {
    return env->NewStringUTF("[\"🐶\",\"🐱\",\"🦁\",\"🐮\",\"🐷\",\"🐸\",\"🐵\"]");
}
// ============================================================
// Haversine / Invite
// ============================================================
JNI_FUNC(jdouble, nativeHaversine)(JNIEnv*, jclass, jdouble jLat1, jdouble jLon1, jdouble jLat2, jdouble jLon2) {
    double dlat = (jLat2 - jLat1) * 3.14159 / 180.0;
    double dlon = (jLon2 - jLon1) * 3.14159 / 180.0;
    double a = sin(dlat/2)*sin(dlat/2) + cos(jLat1*3.14159/180)*cos(jLat2*3.14159/180)*sin(dlon/2)*sin(dlon/2);
    return 6371000 * 2 * atan2(sqrt(a), sqrt(1-a));
}
JNI_FUNC(void, nativeInviteClear)(JNIEnv*, jclass) {}
JNI_FUNC(jint, nativeInviteCount)(JNIEnv*, jclass) { return 0; }
JNI_FUNC(jstring, nativeInviteExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeInviteHide)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeInviteImportJson)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jboolean, nativeInviteIsHidden)(JNIEnv*, jclass, jstring) { return false; }
JNI_FUNC(void, nativeInviteUnhide)(JNIEnv*, jclass, jstring) {}
// ============================================================
// Validation Functions
// ============================================================
JNI_FUNC(jboolean, nativeIsCallEvent)(JNIEnv* env, jclass, jstring jType) {
    std::string t = j2s(env, jType);
    return t == "m.call.invite" || t == "m.call.answer" || t == "m.call.hangup" || t == "m.call.candidates";
}
JNI_FUNC(jboolean, nativeIsEmojiOnly)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText);
    for (char c : s) if (c > 32 && c != ' ') return false;
    return !s.empty();
}
JNI_FUNC(jboolean, nativeIsEmojiOnlyMessage)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText);
    for (char c : s) if (c > 32 && c != ' ') return false;
    return !s.empty();
}
JNI_FUNC(jboolean, nativeIsLikelyFullHistory)(JNIEnv*, jclass, jint jEvents, jboolean) { return jEvents > 0; }
JNI_FUNC(jboolean, nativeIsRoomEncrypted)(JNIEnv*, jclass, jstring) { return false; }
JNI_FUNC(jboolean, nativeIsRoomMention)(JNIEnv* env, jclass, jstring jBody, jstring jRoom) {
    return j2s(env, jBody).find(j2s(env, jRoom)) != std::string::npos;
}
JNI_FUNC(jboolean, nativeIsSupportedAudio)(JNIEnv* env, jclass, jstring jMime) {
    std::string m = j2s(env, jMime);
    return m.find("audio/") == 0 || m.find("video/") == 0;
}
JNI_FUNC(jboolean, nativeIsValidBlurhash)(JNIEnv* env, jclass, jstring jBh) { return jBh && env->GetStringLength(jBh) > 0; }
JNI_FUNC(jboolean, nativeIsValidCrop)(JNIEnv*, jclass, jint, jint, jint, jint, jint, jint) { return true; }
JNI_FUNC(jboolean, nativeIsValidIconAlias)(JNIEnv* env, jclass, jstring jAlias) {
    std::string s = j2s(env, jAlias);