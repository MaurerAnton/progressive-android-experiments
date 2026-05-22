#pragma once
#include <string>
#include <vector>

namespace progressive {

struct EventRelation {
    std::string type;           // "m.annotation", "m.reference", "m.replace", "m.thread"
    std::string eventId;        // related event ID
    std::string key;            // for m.annotation (reaction emoji)
    bool isFallingBack = false;
};

// Parse m.relates_to from event content
EventRelation parseEventRelation(const std::string& json);

// Check relation types
bool isReaction(const EventRelation& rel);
bool isEdit(const EventRelation& rel);
bool isThreadReply(const EventRelation& rel);
bool isReference(const EventRelation& rel);

// Build relation JSON for various types
std::string buildReactionRelation(const std::string& eventId, const std::string& emoji);
std::string buildEditRelation(const std::string& eventId);
std::string buildThreadRelation(const std::string& eventId, bool fallingBack = false);
std::string buildReferenceRelation(const std::string& eventId);

// Get related event IDs from an event
std::vector<std::string> getRelatedEventIds(const std::string& json);

} // namespace progressive
