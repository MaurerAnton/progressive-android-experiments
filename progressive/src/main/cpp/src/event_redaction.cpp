#include "progressive/event_redaction.hpp"
#include <sstream>

namespace progressive {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    if (e == std::string::npos) return "";
    return json.substr(p, e - p);
}

RedactionInfo parseRedaction(const std::string& json) {
    RedactionInfo r;
    r.redactsEventId = extractField(json, "redacts");
    r.reason = extractField(json, "reason");
    r.isValid = !r.redactsEventId.empty();
    return r;
}

bool isRedactionEvent(const std::string& json) {
    return json.find("\"type\":\"m.room.redaction\"") != std::string::npos;
}

std::string buildRedactionContent(const std::string& eventIdToRedact, const std::string& reason) {
    std::ostringstream os;
    os << R"({"redacts":")" << eventIdToRedact << R"(")";
    if (!reason.empty()) os << R"(,"reason":")" << reason << R"(")";
    os << "}";
    return os.str();
}

std::string applyRedaction(const std::string& content) {
    // Matrix spec: redacted events keep only: event_id, type, room_id, sender,
    // origin_server_ts, unsigned, redacts (if redaction)
    // All other fields are stripped
    return R"({"redacted_by":"(redacted)"})";
}

bool isRedactedEvent(const std::string& json) {
    return json.find("\"redacted_because\"") != std::string::npos;
}

std::string formatRedactionNotice(const RedactionInfo& info, const std::string& sender) {
    std::ostringstream os;
    os << sender << " removed a message";
    if (!info.reason.empty()) os << ": " << info.reason;
    return os.str();
}



std::string getRedactionReason(const std::string& json) {
    return parseRedaction(json).reason;
}

bool canRedactEvent(const std::string& senderId, const std::string& myUserId,
                      const std::string& roomId, int myPowerLevel, int requiredPl) {
    if (senderId == myUserId) return true;
    return myPowerLevel >= requiredPl;
}

bool isRedactedByServer(const std::string& json) {
    return json.find(""redacted_because"") != std::string::npos;
}

std::string formatRedactedContent(const std::string& originalSender) {
    return originalSender + " removed a message";
}

} // namespace progressive
