#include "progressive/event_receipt_utils.hpp"
#include <sstream>
#include <set>

namespace progressive {

std::vector<ReceiptInfo> parseReadReceipts(const std::string& json) {
    std::vector<ReceiptInfo> receipts; size_t pos = 0;
    while ((pos = json.find("\"@", pos)) != std::string::npos) {
        auto colon = json.find(':', pos); if (colon == std::string::npos || colon - pos > 50) { pos++; continue; }
        ReceiptInfo r; r.userId = json.substr(pos + 1, colon - pos - 1);
        auto ts = json.find("\"ts\":", colon); if (ts != std::string::npos && ts - colon < 100) {
            ts += 5; try { r.timestampMs = std::stoll(json.substr(ts)); } catch(...) {}
        }
        receipts.push_back(r); pos = colon + 1;
    }
    return receipts;
}
std::string buildReadReceiptContent(const std::string& eid) {
    return R"({"m.read":{")" + eid + R"(":{}}})";
}
std::string buildFullyReadContent(const std::string& eid) {
    return R"({"m.fully_read":")" + eid + R"("})";
}
int countUniqueReaders(const std::vector<ReceiptInfo>& receipts) {
    std::set<std::string> users;
    for (auto& r : receipts) users.insert(r.userId);
    return (int)users.size();
}
bool hasUserRead(const std::vector<ReceiptInfo>& receipts, const std::string& uid) {
    for (auto& r : receipts) if (r.userId == uid) return true;
    return false;
}
} // namespace progressive
