#include "progressive/draft_manager_utils.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

std::string buildDraftKey(const std::string& roomId) {
    return "draft_" + roomId;
}

std::string serializeDraft(const MessageDraft& d) {
    std::ostringstream os;
    os << "{";
    os << R"("body":")" << d.body << R"(")";
    if (!d.formattedBody.empty()) os << R"(,"formatted_body":")" << d.formattedBody << R"(")";
    if (!d.replyEventId.empty()) os << R"(,"reply_event_id":")" << d.replyEventId << R"(")";
    if (!d.threadRootEventId.empty()) os << R"(,"thread_root_id":")" << d.threadRootEventId << R"(")";
    os << R"(,"saved_at_ms":)" << d.savedAtMs;
    os << "}";
    return os.str();
}

MessageDraft deserializeDraft(const std::string& json, const std::string& roomId) {
    MessageDraft d;
    d.roomId = roomId;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    d.body = extract("body");
    d.formattedBody = extract("formatted_body");
    d.replyEventId = extract("reply_event_id");
    d.threadRootEventId = extract("thread_root_id");
    return d;
}

bool shouldShowDraft(const MessageDraft& d, int64_t maxAgeMs) {
    if (d.body.empty()) return false;
    int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (now - d.savedAtMs) < maxAgeMs;
}

std::string formatDraftPreview(const MessageDraft& d) {
    if (d.body.empty()) return "";
    std::string prefix = "Draft: ";
    if (isDraftReply(d)) prefix = "Draft reply: ";
    if (d.body.size() > 50) return prefix + d.body.substr(0, 50) + "...";
    return prefix + d.body;
}

std::string buildClearDraftContent() { return "{}"; }

bool isDraftReply(const MessageDraft& d) { return !d.replyEventId.empty(); }
bool isDraftInThread(const MessageDraft& d) { return !d.threadRootEventId.empty(); }



bool hasDraft(const std::string& json) {
    return !json.empty() && json != "{}" && json != "[]";
}

std::string extractDraftBody(const std::string& json) {
    return deserializeDraft(json, "").body;
}

std::string buildDraftSaveRequest(const std::string& roomId, const std::string& body,
                                    const std::string& replyEventId) {
    MessageDraft d;
    d.roomId = roomId;
    d.body = body;
    d.replyEventId = replyEventId;
    d.savedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return serializeDraft(d);
}

std::string formatDraftIndicator(bool hasDraft) {
    return hasDraft ? "[DRAFT] " : "";
}

} // namespace progressive
