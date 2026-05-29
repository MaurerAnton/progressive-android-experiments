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

#define JNI_FUNC(ret, name) extern "C" JNIEXPORT ret JNICALL Java_chat_progressive_app_native_ProgressiveNative_##name

static std::string j2s(JNIEnv* env, jstring s) {
    if (!s) return "";
    const char* c = env->GetStringUTFChars(s, nullptr);
    std::string r(c);
    env->ReleaseStringUTFChars(s, c);
    return r;
}

// Simple JSON builders using escapeJson for safety
static std::string jsonObj(const std::string& pairs) { return "{" + pairs + "}"; }
static std::string jsonPair(const std::string& k, const std::string& v) {
    return "\"" + progressive::escapeJson(k) + "\":\"" + progressive::escapeJson(v) + "\"";
}
static std::string jsonPair(const std::string& k, bool v) {
    return "\"" + progressive::escapeJson(k) + "\":" + (v ? "true" : "false");
}
static std::string jsonPair(const std::string& k, int64_t v) {
    return "\"" + progressive::escapeJson(k) + "\":" + std::to_string(v);
}
static std::string jsonPair(const std::string& k, double v, int prec = 6) {
    std::ostringstream ss; ss.precision(prec); ss << std::fixed << v;
    return "\"" + progressive::escapeJson(k) + "\":" + ss.str();
}
static std::string jsonArr(const std::string& items) { return "[" + items + "]"; }

// ============================================================
// Alarm Engine
// ============================================================
JNI_FUNC(jstring, nativeAlarmGetWeatherAction)(JNIEnv* env, jclass) {
    return env->NewStringUTF(jsonObj(
        jsonPair("action", "weather") + "," +
        jsonPair("description", "Get current weather for alarm location")
    ).c_str());
}

JNI_FUNC(jstring, nativeAlarmCreate)(JNIEnv* env, jclass, jstring jLabel, jstring, jstring, jstring, jboolean) {
    return env->NewStringUTF(jsonObj(
        jsonPair("id", std::to_string(std::time(nullptr))) + "," +
        jsonPair("label", j2s(env, jLabel)) + "," +
        jsonPair("created", true)
    ).c_str());
}
JNI_FUNC(jstring, nativeAlarmListAll)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
JNI_FUNC(jstring, nativeAlarmGetNext)(JNIEnv* env, jclass) {
    return env->NewStringUTF(jsonObj(jsonPair("hasNext", false)).c_str());
}
JNI_FUNC(void, nativeAlarmDelete)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeAlarmDismiss)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(void, nativeAlarmSnooze)(JNIEnv*, jclass, jstring, jint) {}
JNI_FUNC(void, nativeAlarmSetRingtone)(JNIEnv*, jclass, jstring, jstring) {}
JNI_FUNC(void, nativeAlarmLoad)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Avatar History
// ============================================================
JNI_FUNC(jstring, nativeAvatarAddChange)(JNIEnv* env, jclass, jstring jUid, jstring jUrl, jlong jTs) {
    return env->NewStringUTF(jsonObj(
        jsonPair("userId", j2s(env, jUid)) + "," +
        jsonPair("url", j2s(env, jUrl)) + "," +
        jsonPair("timestamp", (int64_t)jTs) + "," +
        jsonPair("added", true)
    ).c_str());
}
JNI_FUNC(void, nativeAvatarClear)(JNIEnv*, jclass, jstring) {}
JNI_FUNC(jstring, nativeAvatarExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

// ============================================================
// HTML Export
// ============================================================
JNI_FUNC(jstring, nativeBuildHtmlExport)(JNIEnv* env, jclass, jstring jTitle, jstring, jstring, jobjectArray jHtmls) {
    std::ostringstream ss;
    ss << "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>"
       << j2s(env, jTitle) << "</title></head><body>";
    jsize n = env->GetArrayLength(jHtmls);
    for (jsize i = 0; i < n; i++)
        ss << j2s(env, (jstring)env->GetObjectArrayElement(jHtmls, i));
    ss << "</body></html>";
    return env->NewStringUTF(ss.str().c_str());
}

// ============================================================
// Build Utilities
// ============================================================
JNI_FUNC(jstring, nativeBuildKeyRequestBody)(JNIEnv* env, jclass, jstring jRm, jstring jSess, jstring jSk, jstring jAlg, jstring jRid, jstring jDid) {
    return env->NewStringUTF(jsonObj(
        jsonPair("action", "request") + "," +
        jsonPair("body", jsonObj(
            jsonPair("room_id", j2s(env, jRm)) + "," +
            jsonPair("session_id", j2s(env, jSess)) + "," +
            jsonPair("sender_key", j2s(env, jSk)) + "," +
            jsonPair("algorithm", j2s(env, jAlg))
        )) + "," +
        jsonPair("request_id", j2s(env, jRid)) + "," +
        jsonPair("requesting_device_id", j2s(env, jDid))
    ).c_str());
}

JNI_FUNC(jstring, nativeBuildLocationContent)(JNIEnv* env, jclass, jstring jBody, jstring jGeo) {
    return env->NewStringUTF(jsonObj(
        jsonPair("msgtype", "m.location") + "," +
        jsonPair("body", j2s(env, jBody)) + "," +
        jsonPair("geo_uri", j2s(env, jGeo))
    ).c_str());
}

JNI_FUNC(jstring, nativeBuildLoginBody)(JNIEnv* env, jclass, jstring jType, jstring jUser, jstring jPass, jstring jTok, jstring jDid, jstring jDn) {
    std::string body = jsonPair("type", j2s(env, jType)) + ",";
    if (jUser) body += "\"identifier\":" + jsonObj(
        jsonPair("type", "m.id.user") + "," +
        jsonPair("user", j2s(env, jUser))
    ) + ",";
    if (jPass) body += jsonPair("password", j2s(env, jPass)) + ",";
    if (jTok) body += jsonPair("token", j2s(env, jTok)) + ",";
    body += jsonPair("device_id", j2s(env, jDid)) + ",";
    body += jsonPair("initial_device_display_name", j2s(env, jDn));
    return env->NewStringUTF(jsonObj(body).c_str());
}

JNI_FUNC(jstring, nativeBuildMasqueradeAlias)(JNIEnv* env, jclass, jstring jReal, jstring jAlias) {
    return env->NewStringUTF(jsonObj(
        jsonPair("real", j2s(env, jReal)) + "," +
        jsonPair("alias", j2s(env, jAlias))
    ).c_str());
}

JNI_FUNC(jstring, nativeBuildMatrixToUrl)(JNIEnv* env, jclass, jstring jUser, jstring jRoom, jstring jEvent) {
    std::string url = "https://matrix.to/#/";
    if (jUser) url += j2s(env, jUser);
    if (jRoom) url += "/" + j2s(env, jRoom);
    if (jEvent) url += "/" + j2s(env, jEvent);
    return env->NewStringUTF(url.c_str());
}

JNI_FUNC(jstring, nativeBuildPinnedEventsContent)(JNIEnv* env, jclass, jobjectArray jIds) {
    std::string items;
    jsize n = env->GetArrayLength(jIds);
    for (jsize i = 0; i < n; i++) {
        if (i > 0) items += ",";
        items += "\"" + progressive::escapeJson(j2s(env, (jstring)env->GetObjectArrayElement(jIds, i))) + "\"";
    }
    return env->NewStringUTF(jsonObj(jsonPair("pinned", jsonArr(items))).c_str());
}

JNI_FUNC(jstring, nativeBuildTranslateRequest)(JNIEnv* env, jclass, jstring jText, jstring jSrc, jstring jTgt, jstring, jstring, jstring jModel) {
    std::string prompt = "Translate from " + j2s(env, jSrc) + " to " + j2s(env, jTgt) + ": " + j2s(env, jText);
    return env->NewStringUTF(jsonObj(
        jsonPair("model", j2s(env, jModel)) + "," +
        "\"messages\":" + jsonArr(jsonObj(
            jsonPair("role", "user") + "," +
            jsonPair("content", prompt)
        ))
    ).c_str());
}

JNI_FUNC(jstring, nativeBuildUserPill)(JNIEnv* env, jclass, jstring jUser, jstring jName) {
    std::ostringstream ss;
    ss << "<a href=\"https://matrix.to/#/" << j2s(env, jUser) << "\">" << j2s(env, jName) << "</a>";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeBuildYggHomeserverUrl)(JNIEnv* env, jclass, jstring jAddr, jint jPort, jboolean jTls) {
    std::ostringstream ss;
    ss << (jTls ? "https" : "http") << "://[" << j2s(env, jAddr) << "]:" << jPort;
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeBuildStaticMap)(JNIEnv* env, jclass, jdouble jLat, jdouble jLon, jint jZoom, jstring jKey) {
    std::ostringstream ss;
    ss << "https://maps.googleapis.com/maps/api/staticmap?center="
       << jLat << "," << jLon << "&zoom=" << jZoom
       << "&size=600x300&markers=color:red|" << jLat << "," << jLon
       << "&key=" << j2s(env, jKey);
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeBuildThumbnailUrl)(JNIEnv* env, jclass, jstring jMxc, jstring jServer, jint jW, jint jH) {
    std::ostringstream ss;
    ss << j2s(env, jServer) << "/_matrix/media/v3/thumbnail/"
       << j2s(env, jMxc) << "?width=" << jW << "&height=" << jH << "&method=scale";
    return env->NewStringUTF(ss.str().c_str());
}

// ============================================================
// Cache Functions
// ============================================================
JNI_FUNC(void, nativeCacheClear)(JNIEnv*, jclass) {}
JNI_FUNC(jstring, nativeCacheGetByRoom)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }
JNI_FUNC(jstring, nativeCacheGetContext)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }
JNI_FUNC(jstring, nativeCacheGetOlderThan)(JNIEnv* env, jclass, jlong) { return env->NewStringUTF("[]"); }
JNI_FUNC(void, nativeCachePut)(JNIEnv*, jclass, jstring, jstring) {}
JNI_FUNC(jint, nativeCacheSize)(JNIEnv*, jclass) { return 0; }
JNI_FUNC(jstring, nativeCacheStatsJson)(JNIEnv* env, jclass) {
    return env->NewStringUTF(jsonObj(
        jsonPair("size", (int64_t)0) + "," +
        jsonPair("entries", (int64_t)0)
    ).c_str());
}
JNI_FUNC(void, nativeCacheTrack)(JNIEnv*, jclass, jstring, jlong) {}

// ============================================================
// Chat Push Down
// ============================================================
JNI_FUNC(jboolean, nativeChatIsPushedDown)(JNIEnv*, jclass) { return false; }
JNI_FUNC(void, nativeChatPushDown)(JNIEnv*, jclass, jint) {}
JNI_FUNC(void, nativeChatPushDownRestore)(JNIEnv*, jclass) {}

// ============================================================
// Event Classifier
// ============================================================
JNI_FUNC(jint, nativeClassifyEvent)(JNIEnv* env, jclass, jstring jType, jstring jMsgType) {
    std::string type = j2s(env, jType);
    if (type == "m.room.message") return 0;      // Message
    if (type == "m.room.member") return 1;        // Member
    if (type == "m.room.encrypted") return 2;     // Encrypted
    if (type == "m.room.redaction") return 3;     // Redaction
    if (type == "m.room.create") return 4;        // Create
    if (type == "m.sticker") return 5;            // Sticker
    if (type == "m.call.invite" || type == "m.call.answer" || type == "m.call.hangup") return 6;
    return 7; // Other
}

// ============================================================
// Compute Utilities
// ============================================================
JNI_FUNC(jstring, nativeComputeE2eeDecoration)(JNIEnv* env, jclass, jstring jEvent, jstring jMyUser, jstring jDevId) {
    return env->NewStringUTF("{}");
}
JNI_FUNC(jint, nativeComputeMessagesPerDay)(JNIEnv*, jclass, jint jTotal, jint jDays) {
    return jDays > 0 ? jTotal / jDays : 0;
}
JNI_FUNC(jstring, nativeComputeNotifPriority)(JNIEnv* env, jclass, jstring, jstring, jstring, jstring) {
    return env->NewStringUTF("low");
}
JNI_FUNC(jstring, nativeComputeProgress)(JNIEnv* env, jclass, jlong jDone, jlong jTotal) {
    std::ostringstream ss;
    int pct = jTotal > 0 ? (int)(jDone * 100 / jTotal) : 0;
    ss << "{\"done\":" << jDone << ",\"total\":" << jTotal << ",\"percent\":" << pct << "}";
    return env->NewStringUTF(ss.str().c_str());
}
JNI_FUNC(jstring, nativeComputeProxyConfig)(JNIEnv* env, jclass, jint jType, jint jPType, jstring jHost, jint jPort, jstring jUser, jstring jPass) {
    return env->NewStringUTF(jsonObj(
        jsonPair("type", std::to_string(jType)) + "," +
        jsonPair("proxyType", std::to_string(jPType)) + "," +
        jsonPair("host", j2s(env, jHost)) + "," +
        jsonPair("port", (int64_t)jPort)
    ).c_str());
}
JNI_FUNC(jstring, nativeComputeReadMarker)(JNIEnv* env, jclass, jstring jLast, jobjectArray jIds, jobjectArray jSenders, jbooleanArray jMention, jbooleanArray jHighlight, jstring jMyUser) {
    return env->NewStringUTF(jsonObj(
        jsonPair("lastReadEventId", j2s(env, jLast)) + "," +
        jsonPair("unreadCount", (int64_t)0) + "," +
        jsonPair("hasUnread", false)
    ).c_str());
}
JNI_FUNC(jstring, nativeComputeReceiptDisplay)(JNIEnv* env, jclass, jstring jReceipt, jstring jAvatars, jstring jNames) {
    return env->NewStringUTF("{}");
}
JNI_FUNC(jint, nativeComputeRetryDelay)(JNIEnv*, jclass, jint jCount, jint jMax) {
    int d = 1000 * (1 << jCount);
    return d > jMax ? jMax : d;
}
JNI_FUNC(jdouble, nativeComputeRmsVolume)(JNIEnv*, jclass, jbyteArray jData, jint jLen) { return 0.0; }
JNI_FUNC(jstring, nativeComputeScrollPlan)(JNIEnv* env, jclass, jint, jint, jint, jboolean) {
    return env->NewStringUTF("{\"action\":\"none\"}");
}
JNI_FUNC(jint, nativeComputeStreak)(JNIEnv*, jclass, jstring) { return 0; }
JNI_FUNC(jint, nativeComputeThumbnail)(JNIEnv*, jclass, jint, jint, jint, jint) { return 0; }

// ============================================================
// Connection Monitor
// ============================================================
JNI_FUNC(jstring, nativeConnMonitorGetStatus)(JNIEnv* env, jclass) {
    return env->NewStringUTF("{\"connected\":false,\"downtimeMs\":0}");
}
JNI_FUNC(void, nativeConnMonitorOnConnected)(JNIEnv*, jclass) {}
JNI_FUNC(void, nativeConnMonitorOnDisconnected)(JNIEnv*, jclass, jlong) {}

// ============================================================
// Contrast / Decrypt / Deleted Archive
// ============================================================
JNI_FUNC(jdouble, nativeContrastRatio)(JNIEnv*, jclass, jint, jint) { return 0.0; }
JNI_FUNC(jstring, nativeDecideRetry)(JNIEnv* env, jclass, jint, jint, jstring) {
    return env->NewStringUTF("{\"shouldRetry\":false}");
}
JNI_FUNC(jstring, nativeDecryptAccount)(JNIEnv* env, jclass, jstring) {
    return env->NewStringUTF("{}");
}
JNI_FUNC(void, nativeDeletedArchiveAdd)(JNIEnv*, jclass, jstring) {}
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
