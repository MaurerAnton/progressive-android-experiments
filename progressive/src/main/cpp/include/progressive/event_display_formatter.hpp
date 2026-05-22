#pragma once
#include <string>
#include <cstdint>

namespace progressive {

enum class EventDisplayType { TEXT, NOTICE, EMOTE, IMAGE, VIDEO, FILE, AUDIO,
                               LOCATION, POLL, STICKER, CALL, ENCRYPTED, STATE, UNKNOWN };

struct EventDisplayInfo {
    std::string eventId;
    std::string senderId;
    std::string senderName;
    std::string body;           // plain text body
    std::string formattedBody;  // HTML body
    EventDisplayType type = EventDisplayType::UNKNOWN;
    int64_t timestampMs = 0;
    bool isEdited = false;
    bool isReply = false;
    std::string replyToEventId;
    bool isThreadRoot = false;
    bool isEncrypted = false;
    std::string encryptionInfo;
    int reactionCount = 0;
    std::string topReaction;    // most used emoji
    bool canRedact = false;
    bool canReply = false;
};

// Detect event display type from JSON
EventDisplayType detectEventType(const std::string& eventJson);

// Format event for timeline display
EventDisplayInfo formatEventDisplay(const std::string& eventJson, const std::string& senderName,
                                      bool canRedact, bool canReply);

// Format sender name with power level badge
std::string formatSenderDisplay(const std::string& name, const std::string& userId,
                                  int powerLevel = 0);

// Format event timestamp for timeline bubble
std::string formatEventBubbleTime(int64_t timestampMs);

// Format edited notice "(edited)"
std::string formatEditedNotice();

// Format reply preview in timeline
std::string formatReplyPreview(const std::string& originalBody, const std::string& originalSender);

// Get event type display icon name
std::string getEventTypeIcon(EventDisplayType type);

} // namespace progressive
