#include "progressive/read_receipts_utils.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

std::vector<ReadReceipt> parseReadReceipts(const std::string& json, const std::string& eventId) {
    std::vector<ReadReceipt> receipts;
    auto readPos = json.find("\"m.read\"");
    if (readPos == std::string::npos) return receipts;
    
    // Find eventId entry
    auto evtPos = json.find("\"" + eventId + "\"", readPos);
    if (evtPos == std::string::npos) return receipts;
    
    auto tsPos = json.find("\"ts\":", evtPos);
    if (tsPos == std::string::npos) tsPos = json.find("\"origin_server_ts\":", evtPos);
    int64_t baseTs = 0;
    if (tsPos != std::string::npos) {
        tsPos = json.find(':', tsPos) + 1;
        while (tsPos < json.size() && json[tsPos] == ' ') tsPos++;
        try { baseTs = std::stoll(json.substr(tsPos)); } catch(...) {}
    }
    
    // Parse user entries
    auto usersPos = json.find("\"m.read\"", readPos + 7);
    if (usersPos == std::string::npos) usersPos = readPos;
    
    size_t pos = readPos;
    while (true) {
        pos = json.find("\"@", pos);
        if (pos == std::string::npos) break;
        auto colon = json.find(':', pos);
        if (colon == std::string::npos) break;
        if (colon - pos < 50) { // valid user ID length
            ReadReceipt r;
            r.userId = json.substr(pos + 1, colon - pos - 1);
            r.eventId = eventId;
            r.timestampMs = baseTs;
            
            auto userTs = json.find("\"ts\":", colon);
            if (userTs != std::string::npos && userTs - colon < 100) {
                auto tsStart = userTs + 5;
                while (tsStart < json.size() && json[tsStart] == ' ') tsStart++;
                try { r.timestampMs = std::stoll(json.substr(tsStart)); } catch(...) {}
            }
            receipts.push_back(r);
        }
        pos = colon + 1;
    }
    
    return receipts;
}

ReceiptDisplayInfo formatReadReceipts(const std::vector<ReadReceipt>& receipts, int maxVisible) {
    ReceiptDisplayInfo info;
    info.totalReaders = (int)receipts.size();
    
    for (const auto& r : receipts) {
        info.readerNames.push_back(r.userId);
        info.readerAvatars.push_back("");
        if (r.timestampMs > info.lastReadTimestampMs) info.lastReadTimestampMs = r.timestampMs;
    }
    
    if (info.totalReaders == 0) {
        info.formattedText = "";
    } else if (info.totalReaders <= maxVisible) {
        std::ostringstream os;
        for (int i = 0; i < info.totalReaders; i++) {
            if (i > 0) os << ", ";
            // Show localpart of userId
            auto uid = receipts[i].userId;
            auto colon = uid.find(':');
            os << (colon != std::string::npos ? uid.substr(1, colon - 1) : uid);
        }
        info.formattedText = "Read by " + os.str();
    } else {
        int shown = maxVisible;
        std::ostringstream os;
        for (int i = 0; i < shown; i++) {
            if (i > 0) os << ", ";
            auto uid = receipts[i].userId;
            auto colon = uid.find(':');
            os << (colon != std::string::npos ? uid.substr(1, colon - 1) : uid);
        }
        os << " and " << (info.totalReaders - shown) << " others";
        info.formattedText = "Read by " + os.str();
    }
    return info;
}

bool isReadByUser(const std::vector<ReadReceipt>& receipts, const std::string& userId) {
    return std::any_of(receipts.begin(), receipts.end(),
        [&](const ReadReceipt& r) { return r.userId == userId; });
}

} // namespace progressive
