#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct MessageDraft {
    std::string roomId;
    std::string body;
    std::string formattedBody;
    std::string replyEventId;        // m.in_reply_to eventId
    std::string threadRootEventId;   // thread root
    int64_t savedAtMs = 0;
};

// Build draft storage key
std::string buildDraftKey(const std::string& roomId);

// Serialize draft to JSON for storage
std::string serializeDraft(const MessageDraft& draft);

// Deserialize draft from JSON
MessageDraft deserializeDraft(const std::string& json, const std::string& roomId);

// Check if draft should be shown (non-empty, not too old)
bool shouldShowDraft(const MessageDraft& draft, int64_t maxAgeMs = 86400000);

// Format draft preview text for room list
std::string formatDraftPreview(const MessageDraft& draft);

// Clear draft for a room
std::string buildClearDraftContent();

// Check if draft is a reply
bool isDraftReply(const MessageDraft& draft);

// Check if draft is in a thread
bool isDraftInThread(const MessageDraft& draft);

} // namespace progressive
