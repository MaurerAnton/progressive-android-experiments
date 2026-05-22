#include "progressive/timeline_render_utils.hpp"
#include <sstream>

namespace progressive {

TimelineItemType classifyTimelineItem(const std::string& json) {
    if (json.empty()) return TimelineItemType::UNKNOWN;
    
    auto typePos = json.find("\"type\":\"");
    if (typePos == std::string::npos) return TimelineItemType::UNKNOWN;
    typePos += 8;
    auto typeEnd = json.find('"', typePos);
    if (typeEnd == std::string::npos) return TimelineItemType::UNKNOWN;
    std::string t = json.substr(typePos, typeEnd - typePos);
    
    if (t == "m.room.message") {
        if (json.find("\"msgtype\":\"m.sticker\"") != std::string::npos) return TimelineItemType::STICKER;
        if (json.find("\"msgtype\":\"m.location\"") != std::string::npos) return TimelineItemType::LOCATION;
        return TimelineItemType::MESSAGE;
    }
    if (t == "m.room.member") return TimelineItemType::MEMBERSHIP;
    if (t == "m.room.create") return TimelineItemType::ROOM_CREATE;
    if (t == "m.room.encryption") return TimelineItemType::ENCRYPTION;
    if (t == "m.room.tombstone") return TimelineItemType::TOMBSTONE;
    if (t == "m.room.redaction") return TimelineItemType::REDACTION;
    if (t == "m.call.invite" || t == "m.call.answer" || t == "m.call.hangup") return TimelineItemType::CALL;
    if (t.find("m.poll") == 0) return TimelineItemType::POLL;
    if (t == "m.widget" || t == "im.vector.modular.widgets") return TimelineItemType::WIDGET;
    return TimelineItemType::STATE_EVENT;
}

bool shouldGroupEvents(const std::string& s1, const std::string& s2,
                        int64_t ts1, int64_t ts2, int64_t maxGapMs) {
    return s1 == s2 && (ts2 - ts1) <= maxGapMs;
}

std::vector<TimelineItemMeta> computeTimelineMeta(
    const std::vector<std::string>& eventIds,
    const std::vector<std::string>& eventJsons,
    const std::vector<std::string>& senderIds,
    const std::vector<int64_t>& timestamps) {
    
    std::vector<TimelineItemMeta> metas;
    std::string lastSender;
    int64_t lastTs = 0;
    int groupIdx = 0;
    
    for (size_t i = 0; i < eventIds.size(); i++) {
        TimelineItemMeta m;
        m.eventId = eventIds[i];
        m.displayIndex = (int)i;
        m.type = classifyTimelineItem(i < eventJsons.size() ? eventJsons[i] : "");
        
        bool grouped = i > 0 && shouldGroupEvents(senderIds[i], lastSender,
                                                     timestamps[i], lastTs);
        m.isGrouped = grouped;
        
        if (grouped) {
            groupIdx++;
            m.showSender = false;
        } else {
            groupIdx = 0;
            m.showSender = true;
        }
        
        m.isGroupStart = !grouped;
        lastSender = senderIds[i];
        lastTs = timestamps[i];
        metas.push_back(m);
    }
    
    // Mark group ends
    for (size_t i = 0; i < metas.size(); i++) {
        bool nextGrouped = i + 1 < metas.size() && metas[i+1].isGrouped;
        if (!nextGrouped && metas[i].isGrouped) metas[i].isGroupEnd = true;
    }
    
    return metas;
}

std::string formatDateSeparator(const std::string& date) {
    return "── " + date + " ──";
}

std::string formatNewMessagesDivider(int count) {
    if (count <= 0) return "";
    return "── " + std::to_string(count) + " new message" + (count > 1 ? "s" : "") + " ──";
}

} // namespace progressive
