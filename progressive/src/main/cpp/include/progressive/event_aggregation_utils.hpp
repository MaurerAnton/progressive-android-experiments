#pragma once
#include <string>
#include <vector>

namespace progressive {

struct AggregatedEvent {
    std::string eventId;
    std::string type;           // "m.room.message", "m.reaction", etc.
    int displayIndex = 0;
    bool isMerged = false;      // grouped with previous event
    std::vector<std::string> mergedEventIds;
};

// Merge consecutive events from same sender within time window
std::vector<AggregatedEvent> aggregateConsecutive(
    const std::vector<std::string>& eventIds,
    const std::vector<std::string>& senderIds,
    const std::vector<int64_t>& timestampsMs,
    int64_t maxGapMs = 300000);  // 5 minutes

// Merge related events (reactions, edits) under their parent
std::vector<AggregatedEvent> aggregateRelated(
    const std::vector<AggregatedEvent>& events,
    const std::vector<std::string>& relationEventIds,
    const std::vector<std::string>& relationTypes);

// Check if two events should be merged
bool shouldMergeEvents(const std::string& sender1, const std::string& sender2,
                        int64_t ts1, int64_t ts2, int64_t maxGapMs);

// Build merged event header text
std::string buildMergedEventHeader(const std::vector<AggregatedEvent>& group);

} // namespace progressive
