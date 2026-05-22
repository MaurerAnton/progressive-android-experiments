#include "progressive/message_edit_utils.hpp"
#include <sstream>

namespace progressive {

std::string buildEditRelation(const std::string& eventId) {
    return R"("m.relates_to":{"rel_type":"m.replace","event_id":")" + eventId + R"("})";
}

std::string buildEditContent(const EditInfo& info) {
    std::ostringstream os;
    os << "{";
    os << R"("msgtype":"m.text",)";
    os << R"("body":" * )" << info.newBody << R"(",)";
    os << R"("m.new_content":{)";
    os << R"("msgtype":"m.text",)";
    os << R"("body":")" << info.newBody << R"(")";
    if (!info.newFormattedBody.empty()) os << R"(,"formatted_body":")" << info.newFormattedBody << R"(")";
    os << "},";
    os << buildEditRelation(info.originalEventId);
    os << "}";
    return os.str();
}

std::string buildEditFallback(const std::string& newBody, const std::string& origBody) {
    return " * " + newBody + "\n * original: " + origBody;
}

EditInfo parseEditInfo(const std::string& json) {
    EditInfo info;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    auto relatesPos = json.find("\"m.relates_to\"");
    if (relatesPos != std::string::npos)
        info.originalEventId = extract("event_id");
    auto newContentPos = json.find("\"m.new_content\"");
    if (newContentPos != std::string::npos) {
        info.newBody = extract("body");
        info.newFormattedBody = extract("formatted_body");
    }
    return info;
}

bool isEditEvent(const std::string& json) {
    return json.find("\"m.replace\"") != std::string::npos &&
           json.find("\"m.new_content\"") != std::string::npos;
}

std::string getEditOriginalEventId(const std::string& json) {
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    return extract("event_id");
}

std::string formatEditHistory(int editCount) {
    if (editCount <= 0) return "";
    return "(edited " + std::to_string(editCount) + " time" + (editCount > 1 ? "s" : "") + ")";
}

std::string buildUnsendContent(const std::string& eventId, const std::string& reason) {
    std::ostringstream os;
    os << R"({"redacts":")" << eventId << R"(")";
    if (!reason.empty()) os << R"(,"reason":")" << reason << R"(")";
    os << "}";
    return os.str();
}

std::string getLatestEditEventId(const std::string& json) {
    auto latestPos = json.find("\"latest_event_id\":\"");
    if (latestPos == std::string::npos) return "";
    latestPos += 18;
    auto end = json.find('"', latestPos);
    return end != std::string::npos ? json.substr(latestPos, end - latestPos) : "";
}

} // namespace progressive
