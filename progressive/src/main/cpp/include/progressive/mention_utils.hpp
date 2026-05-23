#pragma once
#include <string>
#include <vector>
namespace progressive {
struct MentionMatch { std::string userId; int start=0; int end=0; };
std::vector<MentionMatch> extractMentions(const std::string& text);
std::string buildMentionHtml(const std::string& userId, const std::string& displayName);
bool hasRoomMention(const std::string& text);
}
