#pragma once
#include <string>
#include <vector>

namespace progressive {

struct Mention {
    enum Type { USER, ROOM, EVERYONE };
    Type type = USER;
    std::string targetId;       // userId or roomId
    int startIndex = 0;
    int endIndex = 0;
};

// Parse all mentions from text (@user, #room, @room)
std::vector<Mention> parseMentions(const std::string& text);

// Build mention text: "@displayName"
std::string buildMentionText(const std::string& userId, const std::string& displayName);

// Build mention HTML: <a href="...">@name</a>
std::string buildMentionHtml(const std::string& userId, const std::string& displayName);

// Check if text contains a mention of userId
bool containsMention(const std::string& text, const std::string& userId);

// Check if text contains @room mention
bool isRoomMention(const std::string& text);

} // namespace progressive
