#pragma once
#include <string>

namespace progressive {

struct EditInfo {
    std::string originalEventId;  // event being edited
    std::string newBody;
    std::string newFormattedBody;
    std::string fallbackText;     // " * edited message"
};

// Build m.replace relation for edit
std::string buildEditRelation(const std::string& originalEventId);

// Build edit event content
std::string buildEditContent(const EditInfo& info);

// Build fallback body for edits (" * original body")
std::string buildEditFallback(const std::string& newBody, const std::string& originalBody);

// Parse edit info from event content
EditInfo parseEditInfo(const std::string& json);

// Check if event is an edit (m.replace)
bool isEditEvent(const std::string& json);

// Get original event ID from edit event
std::string getEditOriginalEventId(const std::string& json);

// Format edit history for display ("edited 3 times")
std::string formatEditHistory(int editCount);

// Build redaction (unsend) event content
std::string buildUnsendContent(const std::string& eventId, const std::string& reason = "");

// Extract edited event ID from aggregate event
std::string getLatestEditEventId(const std::string& aggregateJson);

} // namespace progressive
