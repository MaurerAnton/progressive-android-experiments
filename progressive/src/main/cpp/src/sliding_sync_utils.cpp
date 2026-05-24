#include "progressive/sliding_sync_utils.hpp"
#include <sstream>

namespace progressive {

std::string formatRangeParam(const SlidingSyncRange& r) {
    std::ostringstream os;
    os << r.start << "-" << r.end;
    return os.str();
}

std::string buildSlidingSyncRequest(const std::string& pos, const std::vector<SlidingSyncRange>& ranges) {
    std::ostringstream os;
    os << "{";
    if (!pos.empty()) os << R"("pos":")" << pos << R"(",)";
    os << R"("lists":{"rooms":{)";
    os << R"("ranges":[[0,20]],)";
    os << R"("sort":["by_notification_count","by_recency"],)";
    os << R"("required_state":[["m.room.name",""],["m.room.avatar",""]],)";
    os << R"("timeline_limit":1)";
    os << "}}";
    os << "}";
    return os.str();
}

std::vector<std::string> parseSlidingSyncRooms(const std::string& json) {
    std::vector<std::string> rooms;
    size_t pos = 0;
    while (pos < json.size()) {
        auto roomPos = json.find("\"!\"", pos);
        if (roomPos == std::string::npos) break;
        auto colon = json.find('\"', roomPos + 2);
        if (colon == std::string::npos) break;
        rooms.push_back(json.substr(roomPos + 1, colon - roomPos - 1));
        pos = colon + 1;
    }
    return rooms;
}

std::string buildSubscriptionKey(const std::string& roomId, int timelineLimit) {
    return "room_sub_" + roomId + "_" + std::to_string(timelineLimit);
}

bool needsMoreRooms(int loaded, int total) {
    return loaded < total;
}

} // namespace progressive
