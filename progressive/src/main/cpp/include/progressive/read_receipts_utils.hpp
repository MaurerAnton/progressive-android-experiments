#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct ReadReceipt {
    std::string userId;
    std::string eventId;
    int64_t timestampMs = 0;
};

struct ReceiptDisplayInfo {
    int totalReaders = 0;
    std::string formattedText;     // "Read by Alice, Bob and 3 others"
    std::vector<std::string> readerNames;
    std::vector<std::string> readerAvatars;
    int64_t lastReadTimestampMs = 0;
};

// Parse m.read receipt from m.receipt event
std::vector<ReadReceipt> parseReadReceipts(const std::string& eventJson, const std::string& eventId);

// Format read receipts for display
ReceiptDisplayInfo formatReadReceipts(const std::vector<ReadReceipt>& receipts,
                                       int maxVisible = 5);

// Check if an event has been read by a specific user
bool isReadByUser(const std::vector<ReadReceipt>& receipts, const std::string& userId);

} // namespace progressive
