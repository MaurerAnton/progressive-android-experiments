#pragma once
#include <string>
#include <cstdint>

namespace progressive {

struct RedactionInfo {
    std::string redactsEventId;    // event being redacted
    std::string reason;            // optional reason
    bool isValid = false;
};

// Parse m.room.redaction event content
RedactionInfo parseRedaction(const std::string& json);

// Check if an event is a redaction
bool isRedactionEvent(const std::string& json);

// Build redaction event content
std::string buildRedactionContent(const std::string& eventIdToRedact, const std::string& reason = "");

// Apply redaction to event content (strips redacted fields)
std::string applyRedaction(const std::string& eventContent);

// Check if event content has been redacted
bool isRedactedEvent(const std::string& eventJson);

// Format redaction notice for timeline
std::string formatRedactionNotice(const RedactionInfo& info, const std::string& originalSender);

} // namespace progressive
