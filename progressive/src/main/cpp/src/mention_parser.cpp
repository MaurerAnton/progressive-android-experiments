#include "progressive/mention_parser.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

std::vector<Mention> parseMentions(const std::string& text) {
    std::vector<Mention> mentions;
    size_t pos = 0;
    while (pos < text.size()) {
        auto at = text.find('@', pos);
        if (at == std::string::npos) break;
        size_t end = at + 1;
        while (end < text.size() && text[end] != ' ' && text[end] != '\n' &&
               text[end] != ',' && text[end] != '.') end++;
        std::string mention = text.substr(at, end - at);
        pos = end;
        
        Mention m;
        m.startIndex = (int)at;
        m.endIndex = (int)end;
        
        if (mention == "@room" || mention == "@everyone") {
            m.type = Mention::EVERYONE;
        } else if (mention.size() > 1) {
            m.type = Mention::USER;
            m.targetId = mention.substr(1);
        }
        mentions.push_back(m);
    }
    return mentions;
}

std::string buildMentionText(const std::string& userId, const std::string& displayName) {
    return "@" + (displayName.empty() ? userId : displayName);
}

std::string buildMentionHtml(const std::string& userId, const std::string& displayName) {
    std::ostringstream os;
    os << "<a href=\"https://matrix.to/#/" << userId << "\">";
    os << "@" << (displayName.empty() ? userId : displayName);
    os << "</a>";
    return os.str();
}

bool containsMention(const std::string& text, const std::string& userId) {
    return text.find("@" + userId) != std::string::npos;
}

bool isRoomMention(const std::string& text) {
    return text.find("@room") != std::string::npos || text.find("@everyone") != std::string::npos;
}

} // namespace progressive
