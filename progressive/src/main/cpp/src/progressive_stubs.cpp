#include "progressive/cross_signing_manager.hpp"
#include "progressive/device_manager_full.hpp"
#include "progressive/poll_manager.hpp"
#include "progressive/room_directory_manager.hpp"
#include "progressive/room_state_manager.hpp"
#include "progressive/server_notice_manager.hpp"
#include "progressive/space_graph.hpp"
#include <sstream>

namespace progressive {

// ---- content_utils free functions (real implementations) ----

std::string buildMxcUri(const std::string& serverName, const std::string& mediaId) {
    return "mxc://" + serverName + "/" + mediaId;
}

std::string ensureCorrectFormattedBodyInTextReply(const std::string& repliedEventBody,
                                                   const std::string& originalBody,
                                                   const std::string& replyFormattedBody) {
    if (repliedEventBody.empty()) return originalBody;
    std::string fallback = "> <" + repliedEventBody + ">\n\n";
    auto pos = replyFormattedBody.find("<mx-reply>");
    if (pos == std::string::npos) {
        pos = replyFormattedBody.find("<blockquote>");
    }
    if (pos != std::string::npos) {
        auto end = replyFormattedBody.find("</mx-reply>", pos);
        if (end == std::string::npos) end = replyFormattedBody.find("</blockquote>", pos);
        if (end != std::string::npos) {
            end = replyFormattedBody.find('>', end);
            if (end != std::string::npos) end++;
            return replyFormattedBody.substr(0, pos) + replyFormattedBody.substr(end) + fallback;
        }
    }
    return fallback + originalBody;
}

std::string extractUsefulTextFromReply(const std::string& repliedBody) {
    if (repliedBody.empty()) return "";
    std::string result;
    bool inTag = false;
    for (size_t i = 0; i < repliedBody.size(); i++) {
        if (repliedBody[i] == '<') {
            inTag = true;
            continue;
        }
        if (inTag) {
            if (repliedBody[i] == '>') inTag = false;
            continue;
        }
        result += repliedBody[i];
    }
    // Limit to ~150 chars
    if (result.size() > 150) result = result.substr(0, 147) + "...";
    return result;
}

std::string formatSpoilerTextFromHtml(const std::string& formattedBody) {
    std::string result;
    bool inTag = false;
    bool inSpoiler = false;
    for (size_t i = 0; i < formattedBody.size(); i++) {
        if (formattedBody[i] == '<') {
            inTag = true;
            // Check for spoiler tag
            if (i + 20 < formattedBody.size()) {
                auto sub = formattedBody.substr(i + 1, 20);
                if (sub.find("data-mx-spoiler") != std::string::npos) inSpoiler = true;
            }
            if (i + 8 < formattedBody.size()) {
                auto sub = formattedBody.substr(i, 8);
                if (sub == "</span>" && inSpoiler) {
                    result += " (spoiler)";
                    inSpoiler = false;
                }
            }
            continue;
        }
        if (inTag) {
            if (formattedBody[i] == '>') inTag = false;
            continue;
        }
        result += formattedBody[i];
    }
    return result;
}

std::string getEditedTargetEventId(const std::string& contentJson) {
    auto pos = contentJson.find("\"m.relates_to\"");
    if (pos == std::string::npos) return "";
    auto eventPos = contentJson.find("\"event_id\"", pos);
    if (eventPos == std::string::npos) return "";
    auto start = contentJson.find('"', eventPos + 11);
    if (start == std::string::npos) return "";
    auto end = contentJson.find('"', start + 1);
    if (end == std::string::npos) return "";
    return contentJson.substr(start + 1, end - start - 1);
}

std::string getExtensionFromMimeType(const std::string& mimetype) {
    if (mimetype == "image/jpeg") return ".jpg";
    if (mimetype == "image/png")  return ".png";
    if (mimetype == "image/gif")  return ".gif";
    if (mimetype == "image/webp") return ".webp";
    if (mimetype == "image/svg+xml") return ".svg";
    if (mimetype == "video/mp4")  return ".mp4";
    if (mimetype == "video/webm") return ".webm";
    if (mimetype == "audio/mp4")  return ".m4a";
    if (mimetype == "audio/mp3")  return ".mp3";
    if (mimetype == "audio/ogg")  return ".ogg";
    if (mimetype == "audio/wav")  return ".wav";
    if (mimetype == "audio/flac") return ".flac";
    if (mimetype == "text/plain") return ".txt";
    if (mimetype == "application/pdf") return ".pdf";
    if (mimetype == "application/zip") return ".zip";
    return "";
}

std::string getLatestEditEventId(const std::string& editSummaryJson, const std::string& originalEventId) {
    auto pos = editSummaryJson.find("\"latest_event_id\"");
    if (pos == std::string::npos) return originalEventId;
    auto start = editSummaryJson.find('"', pos + 18);
    if (start == std::string::npos) return originalEventId;
    auto end = editSummaryJson.find('"', start + 1);
    if (end == std::string::npos) return originalEventId;
    return editSummaryJson.substr(start + 1, end - start - 1);
}

bool hasTextWithImage(const std::string& contentJson) {
    auto bodyPos = contentJson.find("\"body\"");
    if (bodyPos == std::string::npos) return false;
    auto msgTypePos = contentJson.find("\"msgtype\"");
    if (msgTypePos == std::string::npos) return false;
    bool isImage = contentJson.find("\"m.image\"", msgTypePos) != std::string::npos ||
                   contentJson.find("\"m.image\"", msgTypePos + 20) != std::string::npos;
    if (!isImage) return false;
    auto start = contentJson.find('"', bodyPos + 7);
    if (start == std::string::npos) return false;
    auto end = contentJson.find('"', start + 1);
    if (end == std::string::npos) return false;
    std::string body = contentJson.substr(start + 1, end - start - 1);
    return !body.empty();
}

std::string normalizeMimeType(const std::string& mimeType) {
    if (mimeType.empty()) return mimeType;
    auto slash = mimeType.find('/');
    if (slash == std::string::npos) return mimeType;
    std::string result = mimeType;
    std::transform(result.begin() + slash + 1, result.end(), result.begin() + slash + 1, [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServerUrl,
                                     int width, int height, const std::string& method) {
    const std::string prefix = "mxc://";
    if (mxcUrl.compare(0, prefix.size(), prefix) != 0) return mxcUrl;
    auto slash = mxcUrl.find('/', prefix.size());
    if (slash == std::string::npos) return mxcUrl;
    auto server = mxcUrl.substr(prefix.size(), slash - prefix.size());
    auto mediaId = mxcUrl.substr(slash + 1);
    if (server.empty() || mediaId.empty()) return mxcUrl;
    std::string base = homeServerUrl;
    while (!base.empty() && base.back() == '/') base.pop_back();
    std::ostringstream out;
    out << base << "/_matrix/media/v3/thumbnail/" << server << "/" << mediaId;
    out << "?width=" << width << "&height=" << height << "&method=" << method;
    return out.str();
}

} // namespace progressive

// JNI extensions
#include "progressive/string_utils.hpp"
#include <jni.h>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#define JNI_FUNC2(ret, name) extern "C" JNIEXPORT ret JNICALL Java_chat_progressive_app_native_ProgressiveNative_##name

static std::string j2s(JNIEnv* env, jstring s) {
    if (!s) return "";
    const char* c = env->GetStringUTFChars(s, nullptr);
    std::string r(c);
    env->ReleaseStringUTFChars(s, c);
    return r;
}

static std::string jsonObj(const std::string& p) { return "{" + p + "}"; }
static std::string jsonPair(const std::string& k, const std::string& v) {
    return "\"" + progressive::escapeJson(k) + "\":\"" + progressive::escapeJson(v) + "\"";
}
static std::string jsonPair(const std::string& k, bool v) {
    return "\"" + progressive::escapeJson(k) + "\":" + (v ? "true" : "false");
}
static std::string jsonPair(const std::string& k, int64_t v) {
    return "\"" + progressive::escapeJson(k) + "\":" + std::to_string(v);
}
static std::string jsonArr(const std::string& items) { return "[" + items + "]"; }


// Part 1
JNI_FUNC2(jstring, nativeAlarmCreate)(JNIEnv* env, jclass, jstring jLabel, jstring, jstring, jstring, jboolean) {
    return env->NewStringUTF(jsonObj(
        jsonPair("id", std::to_string(std::time(nullptr))) + "," +
        jsonPair("label", j2s(env, jLabel)) + "," +
        jsonPair("created", true)
    ).c_str());
}

JNI_FUNC2(jstring, nativeAlarmListAll)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jstring, nativeAlarmGetNext)(JNIEnv* env, jclass) {
    return env->NewStringUTF(jsonObj(jsonPair("hasNext", false)).c_str());
}

JNI_FUNC2(void, nativeAlarmDelete)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeAlarmDismiss)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeAlarmSnooze)(JNIEnv*, jclass, jstring, jint) {}

JNI_FUNC2(void, nativeAlarmSetRingtone)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC2(void, nativeAlarmLoad)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Avatar History
// ============================================================

JNI_FUNC2(jstring, nativeAvatarAddChange)(JNIEnv* env, jclass, jstring jUid, jstring jUrl, jlong jTs) {
    return env->NewStringUTF(jsonObj(
        jsonPair("userId", j2s(env, jUid)) + "," +
        jsonPair("url", j2s(env, jUrl)) + "," +
        jsonPair("timestamp", (int64_t)jTs) + "," +
        jsonPair("added", true)
    ).c_str());
}

JNI_FUNC2(void, nativeAvatarClear)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeAvatarExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

// ============================================================
// HTML Export
// ============================================================

JNI_FUNC2(jstring, nativeBuildHtmlExport)(JNIEnv* env, jclass, jstring jTitle, jstring, jstring, jobjectArray jHtmls) {
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

JNI_FUNC2(jstring, nativeBuildKeyRequestBody)(JNIEnv* env, jclass, jstring jRm, jstring jSess, jstring jSk, jstring jAlg, jstring jRid, jstring jDid) {
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

JNI_FUNC2(jstring, nativeBuildLocationContent)(JNIEnv* env, jclass, jstring jBody, jstring jGeo) {
    return env->NewStringUTF(jsonObj(
        jsonPair("msgtype", "m.location") + "," +
        jsonPair("body", j2s(env, jBody)) + "," +
        jsonPair("geo_uri", j2s(env, jGeo))
    ).c_str());
}

JNI_FUNC2(jstring, nativeBuildLoginBody)(JNIEnv* env, jclass, jstring jType, jstring jUser, jstring jPass, jstring jTok, jstring jDid, jstring jDn) {
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

JNI_FUNC2(jstring, nativeBuildMasqueradeAlias)(JNIEnv* env, jclass, jstring jReal, jstring jAlias) {
    return env->NewStringUTF(jsonObj(
        jsonPair("real", j2s(env, jReal)) + "," +
        jsonPair("alias", j2s(env, jAlias))
    ).c_str());
}

JNI_FUNC2(jstring, nativeBuildMatrixToUrl)(JNIEnv* env, jclass, jstring jUser, jstring jRoom, jstring jEvent) {
    std::string url = "https://matrix.to/#/";
    if (jUser) url += j2s(env, jUser);
    if (jRoom) url += "/" + j2s(env, jRoom);
    if (jEvent) url += "/" + j2s(env, jEvent);
    return env->NewStringUTF(url.c_str());
}

JNI_FUNC2(jstring, nativeBuildPinnedEventsContent)(JNIEnv* env, jclass, jobjectArray jIds) {
    std::string items;
    jsize n = env->GetArrayLength(jIds);
    for (jsize i = 0; i < n; i++) {
        if (i > 0) items += ",";
        items += "\"" + progressive::escapeJson(j2s(env, (jstring)env->GetObjectArrayElement(jIds, i))) + "\"";
    }
    return env->NewStringUTF(jsonObj(jsonPair("pinned", jsonArr(items))).c_str());
}

JNI_FUNC2(jstring, nativeBuildTranslateRequest)(JNIEnv* env, jclass, jstring jText, jstring jSrc, jstring jTgt, jstring, jstring, jstring jModel) {
    std::string prompt = "Translate from " + j2s(env, jSrc) + " to " + j2s(env, jTgt) + ": " + j2s(env, jText);
    return env->NewStringUTF(jsonObj(
        jsonPair("model", j2s(env, jModel)) + "," +
        "\"messages\":" + jsonArr(jsonObj(
            jsonPair("role", "user") + "," +
            jsonPair("content", prompt)
        ))
    ).c_str());
}

JNI_FUNC2(jstring, nativeBuildUserPill)(JNIEnv* env, jclass, jstring jUser, jstring jName) {
    std::ostringstream ss;
    ss << "<a href=\"https://matrix.to/#/" << j2s(env, jUser) << "\">" << j2s(env, jName) << "</a>";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeBuildYggHomeserverUrl)(JNIEnv* env, jclass, jstring jAddr, jint jPort, jboolean jTls) {
    std::ostringstream ss;
    ss << (jTls ? "https" : "http") << "://[" << j2s(env, jAddr) << "]:" << jPort;
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeBuildStaticMap)(JNIEnv* env, jclass, jdouble jLat, jdouble jLon, jint jZoom, jstring jKey) {
    std::ostringstream ss;
    ss << "https://maps.googleapis.com/maps/api/staticmap?center="
       << jLat << "," << jLon << "&zoom=" << jZoom
       << "&size=600x300&markers=color:red|" << jLat << "," << jLon
       << "&key=" << j2s(env, jKey);
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeBuildThumbnailUrl)(JNIEnv* env, jclass, jstring jMxc, jstring jServer, jint jW, jint jH) {
    std::ostringstream ss;
    ss << j2s(env, jServer) << "/_matrix/media/v3/thumbnail/"
       << j2s(env, jMxc) << "?width=" << jW << "&height=" << jH << "&method=scale";
    return env->NewStringUTF(ss.str().c_str());
}

// ============================================================
// Cache Functions
// ============================================================

JNI_FUNC2(void, nativeCacheClear)(JNIEnv*, jclass) {}

JNI_FUNC2(jstring, nativeCacheGetByRoom)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jstring, nativeCacheGetContext)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jstring, nativeCacheGetOlderThan)(JNIEnv* env, jclass, jlong) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeCachePut)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC2(jint, nativeCacheSize)(JNIEnv*, jclass) { return 0; }

JNI_FUNC2(jstring, nativeCacheStatsJson)(JNIEnv* env, jclass) {
    return env->NewStringUTF(jsonObj(
        jsonPair("size", (int64_t)0) + "," +
        jsonPair("entries", (int64_t)0)
    ).c_str());
}

JNI_FUNC2(void, nativeCacheTrack)(JNIEnv*, jclass, jstring, jlong) {}

// ============================================================
// Chat Push Down
// ============================================================

JNI_FUNC2(jboolean, nativeChatIsPushedDown)(JNIEnv*, jclass) { return false; }

JNI_FUNC2(void, nativeChatPushDown)(JNIEnv*, jclass, jint) {}

JNI_FUNC2(void, nativeChatPushDownRestore)(JNIEnv*, jclass) {}

// ============================================================
// Event Classifier
// ============================================================

JNI_FUNC2(jint, nativeClassifyEvent)(JNIEnv* env, jclass, jstring jType, jstring jMsgType) {
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

JNI_FUNC2(jstring, nativeComputeE2eeDecoration)(JNIEnv* env, jclass, jstring jEvent, jstring jMyUser, jstring jDevId) {
    return env->NewStringUTF("{}");
}

JNI_FUNC2(jint, nativeComputeMessagesPerDay)(JNIEnv*, jclass, jint jTotal, jint jDays) {
    return jDays > 0 ? jTotal / jDays : 0;
}

JNI_FUNC2(jstring, nativeComputeNotifPriority)(JNIEnv* env, jclass, jstring, jstring, jstring, jstring) {
    return env->NewStringUTF("low");
}

JNI_FUNC2(jstring, nativeComputeProgress)(JNIEnv* env, jclass, jlong jDone, jlong jTotal) {
    std::ostringstream ss;
    int pct = jTotal > 0 ? (int)(jDone * 100 / jTotal) : 0;
    ss << "{\"done\":" << jDone << ",\"total\":" << jTotal << ",\"percent\":" << pct << "}";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeComputeProxyConfig)(JNIEnv* env, jclass, jint jType, jint jPType, jstring jHost, jint jPort, jstring jUser, jstring jPass) {
    return env->NewStringUTF(jsonObj(
        jsonPair("type", std::to_string(jType)) + "," +
        jsonPair("proxyType", std::to_string(jPType)) + "," +
        jsonPair("host", j2s(env, jHost)) + "," +
        jsonPair("port", (int64_t)jPort)
    ).c_str());
}

JNI_FUNC2(jstring, nativeComputeReadMarker)(JNIEnv* env, jclass, jstring jLast, jobjectArray jIds, jobjectArray jSenders, jbooleanArray jMention, jbooleanArray jHighlight, jstring jMyUser) {
    return env->NewStringUTF(jsonObj(
        jsonPair("lastReadEventId", j2s(env, jLast)) + "," +
        jsonPair("unreadCount", (int64_t)0) + "," +
        jsonPair("hasUnread", false)
    ).c_str());
}

JNI_FUNC2(jstring, nativeComputeReceiptDisplay)(JNIEnv* env, jclass, jstring jReceipt, jstring jAvatars, jstring jNames) {
    return env->NewStringUTF("{}");
}

JNI_FUNC2(jint, nativeComputeRetryDelay)(JNIEnv*, jclass, jint jCount, jint jMax) {
    int d = 1000 * (1 << jCount);
    return d > jMax ? jMax : d;
}

JNI_FUNC2(jdouble, nativeComputeRmsVolume)(JNIEnv*, jclass, jbyteArray jData, jint jLen) { return 0.0; }

JNI_FUNC2(jstring, nativeComputeScrollPlan)(JNIEnv* env, jclass, jint, jint, jint, jboolean) {
    return env->NewStringUTF("{\"action\":\"none\"}");
}

JNI_FUNC2(jint, nativeComputeStreak)(JNIEnv*, jclass, jstring) { return 0; }

// Part 2
JNI_FUNC2(jdouble, nativeContrastRatio)(JNIEnv*, jclass, jint, jint) { return 0.0; }

JNI_FUNC2(jstring, nativeDecideRetry)(JNIEnv* env, jclass, jint, jint, jstring) {
    return env->NewStringUTF("{\"shouldRetry\":false}");
}

JNI_FUNC2(jstring, nativeDecryptAccount)(JNIEnv* env, jclass, jstring) {
    return env->NewStringUTF("{}");
}

JNI_FUNC2(void, nativeDeletedArchiveAdd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeDeletedArchiveClear)(JNIEnv*, jclass) {}

JNI_FUNC2(jint, nativeDeletedArchiveCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC2(jstring, nativeDeletedArchiveExportJson)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[]");
}

// ============================================================
// Desync Detector / Language / Drawing
// ============================================================

JNI_FUNC2(jstring, nativeDesyncCheck)(JNIEnv* env, jclass, jstring) {
    return env->NewStringUTF("{\"desynced\":false}");
}

JNI_FUNC2(void, nativeDesyncTrackEvent)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC2(jstring, nativeDetectLanguage)(JNIEnv* env, jclass, jstring jText) {
    return env->NewStringUTF(jsonObj(jsonPair("language", "en")).c_str());
}

JNI_FUNC2(void, nativeDrawClear)(JNIEnv*, jclass) {}

JNI_FUNC2(jstring, nativeDrawExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeDrawLineTo)(JNIEnv*, jclass, jfloat, jfloat) {}

JNI_FUNC2(void, nativeDrawMoveTo)(JNIEnv*, jclass, jfloat, jfloat) {}

JNI_FUNC2(void, nativeDrawSetColor)(JNIEnv*, jclass, jint, jint, jint, jint) {}

JNI_FUNC2(void, nativeDrawSetStrokeWidth)(JNIEnv*, jclass, jfloat) {}

// ============================================================
// Edit Distance / Emoji Blacklist
// ============================================================

JNI_FUNC2(jint, nativeEditDistance)(JNIEnv* env, jclass, jstring jA, jstring jB) {
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

JNI_FUNC2(void, nativeEmojiBlacklistAdd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeEmojiBlacklistExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeEmojiBlacklistImport)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jboolean, nativeEmojiBlacklistIsBlocked)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC2(void, nativeEmojiBlacklistRemove)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Event Extractors
// ============================================================

JNI_FUNC2(jstring, nativeEncryptAccount)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jint, nativeEstimatePaginationRequests)(JNIEnv*, jclass, jint jMiss, jint jPage) { return jPage>0 ? (jMiss+jPage-1)/jPage : 0; }

JNI_FUNC2(jstring, nativeExtractEventLinks)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jstring, nativeExtractFirstUrl)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }

JNI_FUNC2(jstring, nativeExtractServerName)(JNIEnv* env, jclass, jstring jMxid) {
    std::string s = j2s(env, jMxid);
    auto p = s.find(':');
    return env->NewStringUTF((p != std::string::npos ? s.substr(p+1) : "").c_str());
}

JNI_FUNC2(jstring, nativeExtractServers)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

// ============================================================
// File Utilities
// ============================================================

JNI_FUNC2(jboolean, nativeFileHasMetadata)(JNIEnv*, jclass, jstring) { return false; }

// ============================================================
// Format Utilities
// ============================================================

JNI_FUNC2(jstring, nativeFormatChatTimestamp)(JNIEnv* env, jclass, jlong jTs, jboolean j12h) {
    time_t t = jTs / 1000;
    struct tm* tm = localtime(&t);
    char buf[32];
    strftime(buf, sizeof(buf), j12h ? "%I:%M %p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC2(jstring, nativeFormatCreationDate)(JNIEnv* env, jclass, jlong jTs) {
    time_t t = jTs / 1000; struct tm* tm = localtime(&t);
    char buf[32]; strftime(buf, sizeof(buf), "%b %d, %Y", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC2(jstring, nativeFormatEmote)(JNIEnv* env, jclass, jstring jSender, jstring jText) {
    std::string s = "* " + j2s(env, jSender) + " " + j2s(env, jText);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC2(jstring, nativeFormatEventHtml)(JNIEnv* env, jclass, jstring jSender, jstring jTs, jstring jBody, jstring jType, jstring jFile, jstring jSize, jstring jRel, jboolean) {
    std::ostringstream ss;
    ss << "<b>" << j2s(env, jSender) << "</b> " << j2s(env, jBody);
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeFormatEventPlainText)(JNIEnv* env, jclass, jstring jSender, jstring jTs, jstring jBody, jstring jType, jstring jFile, jstring jRel) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC2(jstring, nativeFormatFullTimestamp)(JNIEnv* env, jclass, jlong jTs) {
    time_t t = jTs/1000; struct tm* tm = localtime(&t);
    char buf[64]; strftime(buf, sizeof(buf), "%a %b %d %Y %H:%M:%S", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC2(jstring, nativeFormatGroupLabel)(JNIEnv* env, jclass, jstring jName, jint jCnt) {
    std::ostringstream ss; ss << j2s(env, jName) << " (" << jCnt << ")";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeFormatLastMessagePreview)(JNIEnv* env, jclass, jstring jSender, jstring jBody, jboolean, jboolean) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC2(jstring, nativeFormatLenny)(JNIEnv* env, jclass) { return env->NewStringUTF("( ͡° ͜ʖ ͡°)"); }

JNI_FUNC2(jstring, nativeFormatMemoryLabel)(JNIEnv* env, jclass, jlong jBytes) {
    std::ostringstream ss;
    if (jBytes > 1024*1024) ss << (jBytes/(1024*1024)) << " MB";
    else if (jBytes > 1024) ss << (jBytes/1024) << " KB";
    else ss << jBytes << " B";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeFormatMessageStatus)(JNIEnv* env, jclass, jint jSt) {
    switch(jSt) { case 0: return env->NewStringUTF("sending"); case 1: return env->NewStringUTF("sent"); default: return env->NewStringUTF("error"); }
}

JNI_FUNC2(jstring, nativeFormatNotifBadge)(JNIEnv* env, jclass, jint jCnt) {
    return env->NewStringUTF(jCnt > 99 ? "99+" : std::to_string(jCnt).c_str());
}

JNI_FUNC2(jstring, nativeFormatNotifBody)(JNIEnv* env, jclass, jstring jSender, jstring jBody, jstring jRoom) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC2(jstring, nativeFormatNotifTitle)(JNIEnv* env, jclass, jstring jRoom, jint jCnt) {
    std::ostringstream ss; ss << j2s(env, jRoom) << " (" << jCnt << ")";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeFormatPlain)(JNIEnv* env, jclass, jstring jHtml) { return jHtml; } // return as-is for now

JNI_FUNC2(jstring, nativeFormatPresenceLine)(JNIEnv* env, jclass, jstring jUser, jstring jStatus) {
    std::string s = j2s(env, jUser) + " " + j2s(env, jStatus);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC2(jstring, nativeFormatProgressBar)(JNIEnv* env, jclass, jint jPct, jint jW) {
    std::string bar;
    int filled = jPct * jW / 100;
    for (int i = 0; i < jW; i++) bar += (i < filled) ? '#' : '-';
    return env->NewStringUTF(bar.c_str());
}

JNI_FUNC2(jstring, nativeFormatRateLimitMessage)(JNIEnv* env, jclass, jlong jMs) {
    std::ostringstream ss; ss << "Rate limited. Try again in " << (jMs/1000) << "s";
    return env->NewStringUTF(ss.str().c_str());
}

// Part 3
JNI_FUNC2(jstring, nativeFormatRelativeTime)(JNIEnv* env, jclass, jlong jTs) {
    int64_t now = std::time(nullptr) * 1000;
    int64_t diff = now - jTs;
    if (diff < 60000) return env->NewStringUTF("just now");
    if (diff < 3600000) { std::ostringstream ss; ss << (diff/60000) << "m"; return env->NewStringUTF(ss.str().c_str()); }
    if (diff < 86400000) { std::ostringstream ss; ss << (diff/3600000) << "h"; return env->NewStringUTF(ss.str().c_str()); }
    std::ostringstream ss; ss << (diff/86400000) << "d";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeFormatResolvedEvent)(JNIEnv* env, jclass, jstring jEvent) { return jEvent; }

JNI_FUNC2(jstring, nativeFormatRoomSummary)(JNIEnv* env, jclass, jstring jName, jstring jTopic, jboolean, jint jMem, jboolean) {
    std::ostringstream ss; ss << j2s(env, jName) << " - " << j2s(env, jTopic) << " (" << jMem << " members)";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeFormatSessionBadge)(JNIEnv* env, jclass, jint jCnt) {
    return env->NewStringUTF(jCnt > 99 ? "99+" : std::to_string(jCnt).c_str());
}

JNI_FUNC2(jstring, nativeFormatShortTime)(JNIEnv* env, jclass, jlong jTs, jboolean j12h) {
    time_t t = jTs/1000; struct tm* tm = localtime(&t);
    char buf[16]; strftime(buf, sizeof(buf), j12h ? "%I:%M%p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC2(jstring, nativeFormatShrug)(JNIEnv* env, jclass) { return env->NewStringUTF("¯\\_(ツ)_/¯"); }

JNI_FUNC2(jstring, nativeFormatSpoiler)(JNIEnv* env, jclass, jstring jText) {
    std::string s = "[spoiler] " + j2s(env, jText);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC2(jstring, nativeFormatTableFlip)(JNIEnv* env, jclass) { return env->NewStringUTF("(╯°□°）╯︵ ┻━┻"); }

// ============================================================
// Generate / Getters / Validation
// ============================================================

JNI_FUNC2(jstring, nativeGenerateRainbow)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText), out;
    const char* colors[] = {"#FF0000","#FF7F00","#FFFF00","#00FF00","#0000FF","#4B0082","#8F00FF"};
    for (size_t i = 0; i < s.size(); i++)
        out += "<font color=\"" + std::string(colors[i%7]) + "\">" + s[i] + "</font>";
    return env->NewStringUTF(out.c_str());
}

JNI_FUNC2(jstring, nativeGenerateToken)(JNIEnv* env, jclass, jint jLen) {
    std::string s; s.reserve(jLen);
    const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < jLen; i++) s += chars[rand() % 62];
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC2(jstring, nativeGetBadgeText)(JNIEnv* env, jclass, jint jCnt) {
    return env->NewStringUTF(jCnt > 99 ? "99+" : std::to_string(jCnt).c_str());
}

JNI_FUNC2(jstring, nativeGetCommonTimezones)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"UTC\",\"Europe/London\",\"Europe/Paris\",\"Europe/Berlin\",\"Europe/Moscow\",\"America/New_York\",\"America/Los_Angeles\",\"Asia/Tokyo\",\"Asia/Shanghai\",\"Australia/Sydney\"]");
}

JNI_FUNC2(jstring, nativeGetExtensionFromName)(JNIEnv* env, jclass, jstring jName) {
    std::string s = j2s(env, jName);
    auto p = s.rfind('.');
    return env->NewStringUTF((p != std::string::npos) ? s.substr(p+1).c_str() : "");
}

JNI_FUNC2(jstring, nativeGetFileExtension)(JNIEnv* env, jclass, jstring jName) {
    std::string s = j2s(env, jName);
    auto p = s.rfind('.');
    return env->NewStringUTF((p != std::string::npos) ? s.substr(p+1).c_str() : "");
}

JNI_FUNC2(jstring, nativeGetLanguageLabel)(JNIEnv* env, jclass, jstring jCode) {
    return jCode; // Return code as label
}

JNI_FUNC2(jstring, nativeGetMemoryInfo)(JNIEnv* env, jclass) {
    return env->NewStringUTF("{\"available\":0,\"total\":0,\"used\":0}");
}

JNI_FUNC2(jstring, nativeGetQuickReactions)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"❤️\",\"👍\",\"👎\",\"😄\",\"😢\",\"😮\",\"😡\",\"🎉\"]");
}

JNI_FUNC2(jstring, nativeGetRecommendedMediaQuality)(JNIEnv* env, jclass, jint, jboolean, jboolean, jint, jdouble, jdouble) {
    return env->NewStringUTF("medium");
}

JNI_FUNC2(jstring, nativeGetRoomVersionsJson)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\",\"8\",\"9\",\"10\",\"11\"]");
}

JNI_FUNC2(jstring, nativeGetStrippableMimeTypes)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"application/octet-stream\",\"text/plain\"]");
}

JNI_FUNC2(jstring, nativeGetSuggestedMasqueradeNames)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"Element\",\"FluffyChat\",\"Nheko\",\"SchildiChat\",\"Cinny\",\"Hydrogen\"]");
}

JNI_FUNC2(jstring, nativeGetSuggestedRole)(JNIEnv* env, jclass, jint jPl) {
    if (jPl >= 100) return env->NewStringUTF("Admin");
    if (jPl >= 50) return env->NewStringUTF("Moderator");
    return env->NewStringUTF("User");
}

JNI_FUNC2(jstring, nativeGetTrustBadge)(JNIEnv* env, jclass, jstring, jboolean, jboolean) {
    return env->NewStringUTF("unknown");
}

JNI_FUNC2(jstring, nativeGetVerificationEmojis)(JNIEnv* env, jclass, jstring) {
    return env->NewStringUTF("[\"🐶\",\"🐱\",\"🦁\",\"🐮\",\"🐷\",\"🐸\",\"🐵\"]");
}

// ============================================================
// Haversine / Invite
// ============================================================

JNI_FUNC2(jdouble, nativeHaversine)(JNIEnv*, jclass, jdouble jLat1, jdouble jLon1, jdouble jLat2, jdouble jLon2) {
    double dlat = (jLat2 - jLat1) * 3.14159 / 180.0;
    double dlon = (jLon2 - jLon1) * 3.14159 / 180.0;
    double a = sin(dlat/2)*sin(dlat/2) + cos(jLat1*3.14159/180)*cos(jLat2*3.14159/180)*sin(dlon/2)*sin(dlon/2);
    return 6371000 * 2 * atan2(sqrt(a), sqrt(1-a));
}

JNI_FUNC2(void, nativeInviteClear)(JNIEnv*, jclass) {}

JNI_FUNC2(jint, nativeInviteCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC2(jstring, nativeInviteExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeInviteHide)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeInviteImportJson)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jboolean, nativeInviteIsHidden)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC2(void, nativeInviteUnhide)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Validation Functions
// ============================================================

JNI_FUNC2(jboolean, nativeIsCallEvent)(JNIEnv* env, jclass, jstring jType) {
    std::string t = j2s(env, jType);
    return t == "m.call.invite" || t == "m.call.answer" || t == "m.call.hangup" || t == "m.call.candidates";
}

JNI_FUNC2(jboolean, nativeIsEmojiOnly)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText);
    for (char c : s) if (c > 32 && c != ' ') return false;
    return !s.empty();
}

JNI_FUNC2(jboolean, nativeIsEmojiOnlyMessage)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText);
    for (char c : s) if (c > 32 && c != ' ') return false;
    return !s.empty();
}

JNI_FUNC2(jboolean, nativeIsLikelyFullHistory)(JNIEnv*, jclass, jint jEvents, jboolean) { return jEvents > 0; }

JNI_FUNC2(jboolean, nativeIsRoomEncrypted)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC2(jboolean, nativeIsRoomMention)(JNIEnv* env, jclass, jstring jBody, jstring jRoom) {
    return j2s(env, jBody).find(j2s(env, jRoom)) != std::string::npos;
}

JNI_FUNC2(jboolean, nativeIsSupportedAudio)(JNIEnv* env, jclass, jstring jMime) {
    std::string m = j2s(env, jMime);
    return m.find("audio/") == 0 || m.find("video/") == 0;
}

JNI_FUNC2(jboolean, nativeIsValidBlurhash)(JNIEnv* env, jclass, jstring jBh) { return jBh && env->GetStringLength(jBh) > 0; }

JNI_FUNC2(jboolean, nativeIsValidCrop)(JNIEnv*, jclass, jint, jint, jint, jint, jint, jint) { return true; }

JNI_FUNC2(jboolean, nativeIsValidIconAlias)(JNIEnv* env, jclass, jstring jAlias) {
    std::string s = j2s(env, jAlias);
    return s.size() > 0 && s[0] == '#';
}

JNI_FUNC2(jboolean, nativeIsValidMasqueradeName)(JNIEnv* env, jclass, jstring jName) {
    return jName && env->GetStringLength(jName) > 0 && env->GetStringLength(jName) <= 64;
}

JNI_FUNC2(jboolean, nativeIsValidMxcUri)(JNIEnv* env, jclass, jstring jUri) {
    std::string s = j2s(env, jUri);
    return s.find("mxc://") == 0;
}

JNI_FUNC2(jboolean, nativeIsValidMxid)(JNIEnv* env, jclass, jstring jId) {
    std::string s = j2s(env, jId);
    return s.size() > 0 && s[0] == '@' && s.find(':') != std::string::npos;
}

JNI_FUNC2(jboolean, nativeIsValidPin)(JNIEnv* env, jclass, jstring jPin) {
    jsize l = env->GetStringLength(jPin);
    return l >= 4 && l <= 8;
}

JNI_FUNC2(jboolean, nativeIsValidRecoveryKey)(JNIEnv* env, jclass, jstring jKey) { return jKey && env->GetStringLength(jKey) > 0; }

// Part 4
JNI_FUNC2(jstring, nativeKeywordFilterCheck)(JNIEnv* env, jclass, jstring jText, jstring jKeywords) {
    std::string text = j2s(env, jText), kw = j2s(env, jKeywords);
    if (text.empty() || kw.empty()) return env->NewStringUTF("{\"matched\":false}");
    return env->NewStringUTF((std::string("{\"matched\":") + (text.find(kw) != std::string::npos ? "true" : "false") + "}").c_str());
}

JNI_FUNC2(void, nativeKeywordFilterClear)(JNIEnv*, jclass) {}

JNI_FUNC2(jint, nativeKeywordFilterCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC2(jstring, nativeKeywordFilterExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeKeywordFilterLoad)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeLangHideAdd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jboolean, nativeLangHideIsHidden)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC2(void, nativeLatencyRecord)(JNIEnv*, jclass, jstring, jlong) {}

JNI_FUNC2(jstring, nativeLatencyStats)(JNIEnv* env, jclass) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jstring, nativeLatencyStatsText)(JNIEnv* env, jclass) { return env->NewStringUTF(""); }

// ============================================================
// Light Call / Location
// ============================================================

JNI_FUNC2(jboolean, nativeLightCallAssessMemory)(JNIEnv*, jclass) { return true; }

JNI_FUNC2(void, nativeLightCallEnter)(JNIEnv*, jclass) {}

JNI_FUNC2(void, nativeLightCallExit)(JNIEnv*, jclass) {}

JNI_FUNC2(jdouble, nativeLocationDistance)(JNIEnv*, jclass, jdouble, jdouble, jdouble, jdouble) { return 0.0; }

JNI_FUNC2(jstring, nativeLocationFormatGeoJson)(JNIEnv* env, jclass, jdouble jLat, jdouble jLon) {
    std::ostringstream ss; ss << "{\"type\":\"Point\",\"coordinates\":[" << jLon << "," << jLat << "]}";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeLocationFormatMessage)(JNIEnv* env, jclass, jdouble jLat, jdouble jLon, jstring jLabel) {
    std::ostringstream ss;
    ss << "Location: https://www.openstreetmap.org/?mlat=" << jLat << "&mlon=" << jLon;
    return env->NewStringUTF(ss.str().c_str());
}

// ============================================================
// Match / MIME / Mirror / Module
// ============================================================

JNI_FUNC2(jstring, nativeMatchRooms)(JNIEnv* env, jclass, jstring jQuery, jstring jRooms) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jstring, nativeMimeToMsgType)(JNIEnv* env, jclass, jstring jMime) {
    std::string m = j2s(env, jMime);
    if (m.find("image/") == 0) return env->NewStringUTF("m.image");
    if (m.find("video/") == 0) return env->NewStringUTF("m.video");
    if (m.find("audio/") == 0) return env->NewStringUTF("m.audio");
    return env->NewStringUTF("m.file");
}

JNI_FUNC2(void, nativeMirrorAdd)(JNIEnv*, jclass, jstring, jstring, jstring) {}

JNI_FUNC2(jstring, nativeMirrorExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jstring, nativeMirrorFormatMessage)(JNIEnv* env, jclass, jstring jMsg) { return jMsg; }

JNI_FUNC2(jstring, nativeMirrorGenerateDollMxid)(JNIEnv* env, jclass) {
    return env->NewStringUTF("@doll:localhost");
}

JNI_FUNC2(jboolean, nativeMirrorIsActive)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC2(jboolean, nativeMirrorIsValidDoll)(JNIEnv* env, jclass, jstring jDoll) {
    std::string s = j2s(env, jDoll);
    return s.find("@doll:") != std::string::npos;
}

JNI_FUNC2(void, nativeMirrorRemove)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeModuleEnable)(JNIEnv*, jclass, jstring, jboolean) {}

JNI_FUNC2(jboolean, nativeModuleIsEnabled)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC2(jstring, nativeModuleListJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeModuleScanDir)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Message Aggregation / Message Queue / MXID
// ============================================================

JNI_FUNC2(void, nativeMsgAggAdd)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC2(void, nativeMsgAggClear)(JNIEnv*, jclass) {}

JNI_FUNC2(jint, nativeMsgAggCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC2(jstring, nativeMsgAggGetAllJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeMsgQueueEnqueue)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeMsgQueueExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeMsgQueueMarkFailed)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeMsgQueueMarkSent)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jint, nativeMsgQueuePendingCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC2(void, nativeMsgQueueSetOrder)(JNIEnv*, jclass, jobjectArray) {}

JNI_FUNC2(jstring, nativeMxidVisibilityExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

// ============================================================
// Network Stats / Notification Keywords
// ============================================================

JNI_FUNC2(void, nativeNetStatsClear)(JNIEnv*, jclass) {}

JNI_FUNC2(void, nativeNetStatsEnd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeNetStatsStart)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeNetStatsToJson)(JNIEnv* env, jclass) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jstring, nativeNetStatsToText)(JNIEnv* env, jclass) { return env->NewStringUTF(""); }

JNI_FUNC2(void, nativeNotifKeywordAdd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeNotifKeywordCheck)(JNIEnv* env, jclass, jstring, jstring) { return env->NewStringUTF("{\"matched\":false}"); }

// Part 5
JNI_FUNC2(jstring, nativeNotifKeywordExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeNotifKeywordImport)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Parse Functions
// ============================================================

JNI_FUNC2(jstring, nativeParseColor)(JNIEnv* env, jclass, jstring jHex) { return env->NewStringUTF("{\"r\":0,\"g\":0,\"b\":0}"); }

JNI_FUNC2(jstring, nativeParseEncryptedHeader)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jstring, nativeParseGeoUri)(JNIEnv* env, jclass, jstring jUri) {
    std::string u = j2s(env, jUri);
    auto p = u.find(':');
    return env->NewStringUTF((p != std::string::npos) ? u.substr(p+1).c_str() : "");
}

JNI_FUNC2(jstring, nativeParseJumpToDate)(JNIEnv* env, jclass, jstring jArgs) { return jArgs; }

JNI_FUNC2(jstring, nativeParsePinnedEventIds)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jstring, nativeParsePushCondition)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jstring, nativeParseRelation)(JNIEnv* env, jclass, jstring, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jstring, nativeParseResponse)(JNIEnv* env, jclass, jstring, jint) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jstring, nativeParseServerCapabilities)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jstring, nativeParseSvg)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }

JNI_FUNC2(jstring, nativeParseTranslateResponse)(JNIEnv* env, jclass, jstring, jint) { return env->NewStringUTF("{}"); }

// ============================================================
// Profile Swiper / Replacement Rules
// ============================================================

JNI_FUNC2(jstring, nativeProfileSwiperNext)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"hasMore\":false}"); }

JNI_FUNC2(jstring, nativeProfileSwiperPrev)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"hasMore\":false}"); }

JNI_FUNC2(void, nativeProfileSwiperSetProfiles)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeReplacementAddRule)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC2(jstring, nativeReplacementApply)(JNIEnv* env, jclass, jstring jText) { return jText; }

JNI_FUNC2(jstring, nativeReplacementExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

// ============================================================
// Resolve / Sanitize / Scheduled Edits
// ============================================================

JNI_FUNC2(jstring, nativeResolveMatrixId)(JNIEnv* env, jclass, jstring jId) { return jId; }

JNI_FUNC2(jstring, nativeRewriteHomeserverUrl)(JNIEnv* env, jclass, jstring jUrl, jstring jYgg) { return jUrl; }

JNI_FUNC2(jstring, nativeSanitizeRoomName)(JNIEnv* env, jclass, jstring jName) { return jName; }

JNI_FUNC2(void, nativeSchedEditCancel)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeSchedEditExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jstring, nativeSchedEditGetDue)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeSchedEditMarkApplied)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeSchedEditMarkFailed)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeSchedEditSchedule)(JNIEnv* env, jclass, jstring, jstring, jstring, jlong) { return env->NewStringUTF("{\"id\":\"0\"}"); }

JNI_FUNC2(jstring, nativeSchedEditStats)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"total\":0,\"applied\":0,\"failed\":0}"); }

// ============================================================
// Search / SHA / Scroll / Smart Replies
// ============================================================

JNI_FUNC2(void, nativeSearchClear)(JNIEnv*, jclass) {}

JNI_FUNC2(void, nativeSearchIndexMessage)(JNIEnv*, jclass, jstring, jstring, jstring) {}

JNI_FUNC2(jint, nativeSearchIndexedCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC2(jstring, nativeSearchQuery)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jstring, nativeSha256Hex)(JNIEnv* env, jclass, jstring jData) { return jData; }

JNI_FUNC2(jboolean, nativeShouldAutoScroll)(JNIEnv*, jclass, jboolean jOwn, jboolean jAtBottom) { return jAtBottom || jOwn; }

JNI_FUNC2(jboolean, nativeShouldBlockImage)(JNIEnv* env, jclass, jstring jUrl) { return false; }

JNI_FUNC2(jboolean, nativeShouldLock)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC2(jboolean, nativeShouldUseLightweightMode)(JNIEnv*, jclass) { return true; }

JNI_FUNC2(jstring, nativeSmartDefaultReactions)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[\"👍\",\"❤️\",\"😄\"]"); }

JNI_FUNC2(jstring, nativeSmartDefaultReplies)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[\"Yes\",\"No\",\"OK\"]"); }

JNI_FUNC2(jstring, nativeSmartParseReplies)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jstring, nativeSmartReplyPrompt)(JNIEnv* env, jclass, jstring jMsg) { return jMsg; }

// ============================================================
// Strip / Suggest / Symbol / Text / Thread
// ============================================================

JNI_FUNC2(jstring, nativeStripPills)(JNIEnv* env, jclass, jstring jText) { return jText; }

JNI_FUNC2(jint, nativeSuggestBarCount)(JNIEnv*, jclass, jint jW) { return jW / 50 + 1; }

JNI_FUNC2(jstring, nativeSuggestQuietHours)(JNIEnv* env, jclass) { return env->NewStringUTF("22:00-07:00"); }

JNI_FUNC2(void, nativeSymbolAdd)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC2(jstring, nativeSymbolExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

// Part 6
JNI_FUNC2(jdouble, nativeTextSimilarity)(JNIEnv* env, jclass, jstring jA, jstring jB) {
    return j2s(env, jA) == j2s(env, jB) ? 1.0 : 0.0;
}

JNI_FUNC2(void, nativeThreadAdd)(JNIEnv*, jclass, jstring, jstring, jstring) {}

JNI_FUNC2(void, nativeThreadClear)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jint, nativeThreadCount)(JNIEnv*, jclass, jstring) { return 0; }

JNI_FUNC2(jstring, nativeThreadGetAllJson)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeThreadRemoveRoom)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Truncate / Uploader / URL Encode / User Hide
// ============================================================

JNI_FUNC2(jstring, nativeTruncateMessage)(JNIEnv* env, jclass, jstring jMsg, jint jMax) {
    std::string s = j2s(env, jMsg);
    return env->NewStringUTF(s.size() > (size_t)jMax ? (s.substr(0, jMax) + "...").c_str() : s.c_str());
}

JNI_FUNC2(jstring, nativeTruncateText)(JNIEnv* env, jclass, jstring jText, jint jMax) {
    std::string s = j2s(env, jText);
    return env->NewStringUTF(s.size() > (size_t)jMax ? (s.substr(0, jMax) + "...").c_str() : s.c_str());
}

JNI_FUNC2(void, nativeUploaderFail)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeUploaderGetChunkJson)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jstring, nativeUploaderProgressJson)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"done\":0,\"total\":0}"); }

JNI_FUNC2(jstring, nativeUrlDecode)(JNIEnv* env, jclass, jstring jUrl) { return jUrl; }

JNI_FUNC2(jstring, nativeUrlEncode)(JNIEnv* env, jclass, jstring jUrl) { return jUrl; }

JNI_FUNC2(void, nativeUserHideFor)(JNIEnv*, jclass, jstring, jstring, jlong) {}

// ============================================================
// Final batch — remaining 31 functions
// ============================================================

JNI_FUNC2(void, nativeDrawSetWidth)(JNIEnv*, jclass, jfloat) {}

JNI_FUNC2(jstring, nativeDrawToSvgPath)(JNIEnv* env, jclass) { return env->NewStringUTF(""); }

JNI_FUNC2(jstring, nativeFormatTimestamp)(JNIEnv* env, jclass, jlong jTs, jboolean j12h) {
    time_t t = jTs / 1000; struct tm* tm = localtime(&t);
    char buf[32]; strftime(buf, sizeof(buf), j12h ? "%I:%M %p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC2(jstring, nativeFormatTimestampInTimezone)(JNIEnv* env, jclass, jlong jTs, jstring, jboolean j12h) {
    time_t t = jTs / 1000; struct tm* tm = localtime(&t);
    char buf[32]; strftime(buf, sizeof(buf), j12h ? "%I:%M %p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC2(jstring, nativeFormatTypingText)(JNIEnv* env, jclass, jobjectArray jNames, jint jMax) {
    std::ostringstream ss;
    jsize n = env->GetArrayLength(jNames);
    for (jsize i = 0; i < n && i < (jsize)jMax; i++) {
        if (i > 0) ss << ", ";
        ss << j2s(env, (jstring)env->GetObjectArrayElement(jNames, i));
    }
    ss << " typing...";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC2(jstring, nativeFormatUserDisplayName)(JNIEnv* env, jclass, jstring jName, jstring jMxid) {
    return env->NewStringUTF(j2s(env, jName).empty() ? j2s(env, jMxid).c_str() : j2s(env, jName).c_str());
}

JNI_FUNC2(jstring, nativeFormatUserMessagePreview)(JNIEnv* env, jclass, jstring jSender, jstring jBody) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC2(jboolean, nativeIsValidSvg)(JNIEnv* env, jclass, jstring jSvg) {
    std::string s = j2s(env, jSvg);
    return s.find("<svg") != std::string::npos;
}

JNI_FUNC2(jboolean, nativeIsValidTimezoneId)(JNIEnv* env, jclass, jstring jTz) { return jTz && env->GetStringLength(jTz) > 0; }

JNI_FUNC2(jboolean, nativeIsYggdrasilAddress)(JNIEnv* env, jclass, jstring jAddr) {
    std::string s = j2s(env, jAddr);
    return s.find("[") == 0 || s.find("200:") == 0 || s.find("300:") == 0;
}
// Remaining 21 are already covered by simpler functions or are not yet declared

JNI_FUNC2(jboolean, nativeIsYggdrasilDomain)(JNIEnv* env, jclass, jstring jHost) {
    std::string s = j2s(env, jHost);
    return s.find(".ygg") != std::string::npos;
}

JNI_FUNC2(void, nativeMxidVisibilityHide)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jboolean, nativeMxidVisibilityIsVisible)(JNIEnv*, jclass, jstring) { return true; }

JNI_FUNC2(void, nativeMxidVisibilityShow)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeUserHideGetActive)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(jboolean, nativeUserHideIsHidden)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC2(jstring, nativeUserIdToColor)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("#4A90D9"); }

JNI_FUNC2(void, nativeUserMaskClear)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jint, nativeUserMaskCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC2(jstring, nativeUserMaskExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC2(void, nativeUserMaskImportJson)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(void, nativeUserMaskRemove)(JNIEnv*, jclass, jstring) {}

JNI_FUNC2(jstring, nativeUserMaskResolveAvatar)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }

JNI_FUNC2(jstring, nativeUserMaskResolveName)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }

JNI_FUNC2(void, nativeUserMaskSet)(JNIEnv*, jclass, jstring, jstring, jstring) {}

JNI_FUNC2(jstring, nativeValidateAndBuild)(JNIEnv* env, jclass, jstring, jstring, jstring, jstring, jboolean) { return env->NewStringUTF("{}"); }

JNI_FUNC2(jboolean, nativeValidateEvent)(JNIEnv*, jclass, jstring) { return true; }

JNI_FUNC2(jboolean, nativeValidateHomeserverUrl)(JNIEnv* env, jclass, jstring jUrl) {
    std::string s = j2s(env, jUrl);
    return s.find("https://") == 0 || s.find("http://") == 0;
}

JNI_FUNC2(jboolean, nativeValidatePasswordWithUsername)(JNIEnv* env, jclass, jstring jPw, jstring) {
    return env->GetStringLength(jPw) >= 8;
}

JNI_FUNC2(jboolean, nativeValidateUsername)(JNIEnv* env, jclass, jstring jUser) {
    std::string s = j2s(env, jUser);
    return s.size() >= 3 && s.find(" ") == std::string::npos;
}

JNI_FUNC2(jint, nativeWordCount)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText);
    int wc = 0; bool inWord = false;
    for (char c : s) { if (c > 32) { if (!inWord) { wc++; inWord = true; } } else { inWord = false; } }
    return wc;
}
