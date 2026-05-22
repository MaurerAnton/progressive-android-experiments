#include "progressive/call_event_formatter.hpp"
#include <sstream>

namespace progressive {

CallEventType parseCallEventType(const std::string& json) {
    if (json.find("\"m.call.invite\"") != std::string::npos) return CallEventType::INVITE;
    if (json.find("\"m.call.answer\"") != std::string::npos) return CallEventType::ANSWER;
    if (json.find("\"m.call.hangup\"") != std::string::npos) return CallEventType::HANGUP;
    if (json.find("\"m.call.reject\"") != std::string::npos) return CallEventType::REJECT;
    if (json.find("\"m.call.candidates\"") != std::string::npos) return CallEventType::CANDIDATES;
    if (json.find("\"m.call.select_answer\"") != std::string::npos) return CallEventType::SELECT_ANSWER;
    if (json.find("\"m.call.negotiate\"") != std::string::npos) return CallEventType::NEGOTIATE;
    return CallEventType::UNKNOWN;
}

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    return e != std::string::npos ? json.substr(p, e - p) : "";
}

CallEventDisplay formatCallEvent(const std::string& json, const std::string& caller,
                                   const std::string& myId) {
    CallEventDisplay d;
    d.type = parseCallEventType(json);
    d.callerName = caller;
    d.callId = extractField(json, "call_id");
    d.isVideoCall = json.find("\"m.video\"") != std::string::npos;
    d.formattedText = formatCallEventText(d.type, caller, d.isVideoCall, 0, caller == myId);
    return d;
}

std::string formatCallDuration(int seconds) {
    int hrs = seconds / 3600, mins = (seconds % 3600) / 60, secs = seconds % 60;
    std::ostringstream os;
    if (hrs > 0) os << hrs << ":";
    if (mins < 10 && hrs > 0) os << "0";
    os << mins << ":";
    if (secs < 10) os << "0";
    os << secs;
    return os.str();
}

std::string formatCallEventText(CallEventType type, const std::string& caller, bool video,
                                  int duration, bool isMe) {
    std::string who = isMe ? "You" : caller;
    std::string callType = video ? "Video call" : "Call";
    
    switch (type) {
        case CallEventType::INVITE: return who + " started a " + callType;
        case CallEventType::ANSWER: return who + " answered";
        case CallEventType::HANGUP:
            return who + " ended the call" + (duration > 0 ? " (" + formatCallDuration(duration) + ")" : "");
        case CallEventType::REJECT: return who + " missed the call";
        case CallEventType::CANDIDATES: return "Call connecting...";
        default: return callType;
    }
}

std::string getCallEventIcon(CallEventType type, bool video) {
    if (type == CallEventType::REJECT || type == CallEventType::HANGUP)
        return video ? "ic_call_end_video" : "ic_call_end";
    return video ? "ic_call_video" : "ic_call_audio";
}

bool isCallEvent(const std::string& json) {
    return json.find("\"m.call.") != std::string::npos;
}

} // namespace progressive
