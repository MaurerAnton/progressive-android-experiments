#pragma once
#include <string>
#include <cstdint>

namespace progressive {

enum class CallEventType { INVITE, ANSWER, HANGUP, REJECT, CANDIDATES, SELECT_ANSWER, NEGOTIATE, UNKNOWN };

struct CallEventDisplay {
    std::string callId;
    CallEventType type = CallEventType::UNKNOWN;
    std::string callerName;
    bool isVideoCall = false;
    int durationSeconds = 0;
    std::string formattedText;      // display text
    std::string iconName;           // UI icon
};

// Parse call event type from JSON
CallEventType parseCallEventType(const std::string& json);

// Format call event for timeline display
CallEventDisplay formatCallEvent(const std::string& json, const std::string& callerName,
                                   const std::string& myUserId);

// Format call duration ("5:32", "1:02:15")
std::string formatCallDuration(int seconds);

// Format call event text
std::string formatCallEventText(CallEventType type, const std::string& caller, bool isVideo,
                                  int duration, bool isMe);

// Get call event icon
std::string getCallEventIcon(CallEventType type, bool isVideo);

// Check if event is a call event
bool isCallEvent(const std::string& json);

} // namespace progressive
