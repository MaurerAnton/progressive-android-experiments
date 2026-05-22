#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct ReceiptInfo {
    std::string eventId;
    std::string userId;
    int64_t timestampMs = 0;
};

// Parse m.read receipts from event JSON
std::vector<ReceiptInfo> parseReadReceipts(const std::string& json);

// Build m.read receipt content
std::string buildReadReceiptContent(const std::string& eventId);

// Build m.fully_read marker
std::string buildFullyReadContent(const std::string& eventId);

// Count unique readers
int countUniqueReaders(const std::vector<ReceiptInfo>& receipts);

// Check if user has read the event
bool hasUserRead(const std::vector<ReceiptInfo>& receipts, const std::string& userId);

} // namespace progressive
