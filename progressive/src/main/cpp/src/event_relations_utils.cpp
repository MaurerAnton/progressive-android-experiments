#include "progressive/event_relations_utils.hpp"
#include <sstream>

namespace progressive {

EventRelation parseEventRelation(const std::string& json) {
    EventRelation rel;
    auto relPos = json.find("\"m.relates_to\"");
    if (relPos == std::string::npos) return rel;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"", relPos);
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    rel.type = extract("rel_type");
    rel.eventId = extract("event_id");
    rel.key = extract("key");
    rel.isFallingBack = json.find("\"is_falling_back\":true", relPos) != std::string::npos;
    return rel;
}

bool isReaction(const EventRelation& r) { return r.type == "m.annotation" && !r.key.empty(); }
bool isEdit(const EventRelation& r) { return r.type == "m.replace"; }
bool isThreadReply(const EventRelation& r) { return r.type == "m.thread"; }
bool isReference(const EventRelation& r) { return r.type == "m.reference"; }

std::string buildReactionRelation(const std::string& id, const std::string& emoji) {
    std::ostringstream os;
    os << R"("m.relates_to":{"event_id":")" << id << R"(","rel_type":"m.annotation","key":")" << emoji << R"("})";
    return os.str();
}
std::string buildEditRelation(const std::string& id) {
    return R"("m.relates_to":{"event_id":")" + id + R"(","rel_type":"m.replace"})";
}
std::string buildThreadRelation(const std::string& id, bool fb) {
    std::ostringstream os;
    os << R"("m.relates_to":{"event_id":")" << id << R"(","rel_type":"m.thread")";
    if (fb) os << R"(,"is_falling_back":true)";
    os << "}";
    return os.str();
}
std::string buildReferenceRelation(const std::string& id) {
    return R"("m.relates_to":{"event_id":")" + id + R"(","rel_type":"m.reference"})";
}

std::vector<std::string> getRelatedEventIds(const std::string& json) {
    std::vector<std::string> ids;
    size_t pos = 0;
    while ((pos = json.find("\"event_id\":\"", pos)) != std::string::npos) {
        pos += 12; auto end = json.find('"', pos);
        if (end != std::string::npos) ids.push_back(json.substr(pos, end - pos));
        pos = end + 1;
    }
    return ids;
}

} // namespace progressive
