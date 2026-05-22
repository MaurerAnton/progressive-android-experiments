#include "progressive/event_aggregation_utils.hpp"
#include <sstream>

namespace progressive {

std::vector<AggregatedEvent> aggregateConsecutive(
    const std::vector<std::string>& eventIds,
    const std::vector<std::string>& senderIds,
    const std::vector<int64_t>& timestampsMs,
    int64_t maxGapMs) {
    
    std::vector<AggregatedEvent> result;
    if (eventIds.empty()) return result;
    
    int displayIdx = 0;
    std::string lastSender;
    int64_t lastTs = 0;
    size_t mergeStart = 0;
    
    for (size_t i = 0; i < eventIds.size(); i++) {
        bool shouldMerge = i > 0 && senderIds[i] == lastSender &&
                           (timestampsMs[i] - lastTs <= maxGapMs);
        
        if (!shouldMerge && i > 0) {
            // Close previous merge group
            AggregatedEvent merged;
            merged.eventId = eventIds[mergeStart];
            merged.displayIndex = displayIdx++;
            merged.isMerged = (i - mergeStart) > 1;
            for (size_t j = mergeStart; j < i; j++)
                merged.mergedEventIds.push_back(eventIds[j]);
            result.push_back(merged);
            mergeStart = i;
        }
        
        lastSender = senderIds[i];
        lastTs = timestampsMs[i];
    }
    
    // Close last group
    AggregatedEvent merged;
    merged.eventId = eventIds[mergeStart];
    merged.displayIndex = displayIdx++;
    merged.isMerged = (eventIds.size() - mergeStart) > 1;
    for (size_t j = mergeStart; j < eventIds.size(); j++)
        merged.mergedEventIds.push_back(eventIds[j]);
    result.push_back(merged);
    
    return result;
}

std::vector<AggregatedEvent> aggregateRelated(
    const std::vector<AggregatedEvent>& events,
    const std::vector<std::string>& relationEventIds,
    const std::vector<std::string>& relationTypes) {
    // Simple pass-through for now
    return events;
}

bool shouldMergeEvents(const std::string& s1, const std::string& s2,
                        int64_t ts1, int64_t ts2, int64_t maxGapMs) {
    return s1 == s2 && (ts2 - ts1) <= maxGapMs;
}

std::string buildMergedEventHeader(const std::vector<AggregatedEvent>& group) {
    if (group.empty()) return "";
    std::ostringstream os;
    os << group.size() << " merged messages";
    return os.str();
}

} // namespace progressive
