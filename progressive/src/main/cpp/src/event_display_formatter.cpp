#include "progressive/event_display_formatter.hpp"
#include <sstream>
#include <chrono>
#include <ctime>

namespace progressive {

EventDisplayType detectEventType(const std::string& json) {
    if (json.empty()) return EventDisplayType::UNKNOWN;
    
    if (json.find("\"m.room.encrypted\"") != std::string::npos)
        return EventDisplayType::ENCRYPTED;
    if (json.find("\"call_id\"") != std::string::npos || json.find("\"m.call\"") != std::string::npos)
        return EventDisplayType::CALL;
    
    auto msgTypePos = json.find("\"msgtype\":\"");
    if (msgTypePos == std::string::npos) return EventDisplayType::STATE;
    
    msgTypePos += 11;
    auto msgEnd = json.find('"', msgTypePos);
    if (msgEnd == std::string::npos) return EventDisplayType::UNKNOWN;
    std::string msgType = json.substr(msgTypePos, msgEnd - msgTypePos);
    
    if (msgType == "m.text") return EventDisplayType::TEXT;
    if (msgType == "m.notice") return EventDisplayType::NOTICE;
    if (msgType == "m.emote") return EventDisplayType::EMOTE;
    if (msgType == "m.image") return EventDisplayType::IMAGE;
    if (msgType == "m.video") return EventDisplayType::VIDEO;
    if (msgType == "m.file") return EventDisplayType::FILE;
    if (msgType == "m.audio") return EventDisplayType::AUDIO;
    if (msgType == "m.location") return EventDisplayType::LOCATION;
    if (msgType == "m.sticker") return EventDisplayType::STICKER;
    if (json.find("poll") != std::string::npos) return EventDisplayType::POLL;
    return EventDisplayType::TEXT;
}

EventDisplayInfo formatEventDisplay(const std::string& json, const std::string& senderName,
                                      bool canRedact, bool canReply) {
    EventDisplayInfo info;
    info.senderName = senderName;
    info.type = detectEventType(json);
    info.isEncrypted = info.type == EventDisplayType::ENCRYPTED;
    info.canRedact = canRedact;
    info.canReply = canReply;
    
    auto replyPos = json.find("\"m.in_reply_to\"");
    info.isReply = replyPos != std::string::npos;
    info.isEdited = json.find("\"m.replace\"") != std::string::npos;
    
    return info;
}

std::string formatSenderDisplay(const std::string& name, const std::string& userId,
                                  int powerLevel) {
    std::ostringstream os;
    os << (name.empty() ? userId : name);
    if (powerLevel >= 100) os << " 👑";
    return os.str();
}

std::string formatEventBubbleTime(int64_t ms) {
    time_t t = ms / 1000;
    char buf[8];
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(buf, sizeof(buf), "%H:%M", &tm);
    return buf;
}

std::string formatEditedNotice() { return "(edited)"; }

std::string formatReplyPreview(const std::string& body, const std::string& sender) {
    std::string preview = body;
    if (preview.size() > 100) preview = preview.substr(0, 97) + "...";
    return sender + ": " + preview;
}

std::string getEventTypeIcon(EventDisplayType type) {
    switch (type) {
        case EventDisplayType::IMAGE: return "ic_message_image";
        case EventDisplayType::VIDEO: return "ic_message_video";
        case EventDisplayType::FILE: return "ic_message_file";
        case EventDisplayType::AUDIO: return "ic_message_audio";
        case EventDisplayType::LOCATION: return "ic_message_location";
        case EventDisplayType::STICKER: return "ic_message_sticker";
        case EventDisplayType::ENCRYPTED: return "ic_message_encrypted";
        case EventDisplayType::CALL: return "ic_message_call";
        default: return "ic_message_text";
    }
}

} // namespace progressive
