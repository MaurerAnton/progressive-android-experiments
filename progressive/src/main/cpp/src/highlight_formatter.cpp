#include "progressive/highlight_formatter.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

static std::string toLower(const std::string& s) {
    std::string r; r.reserve(s.size());
    std::transform(s.begin(), s.end(), std::back_inserter(r), ::tolower);
    return r;
}

bool hasHighlight(const std::string& text, const std::vector<HighlightRule>& rules) {
    std::string lower = toLower(text);
    for (const auto& r : rules) {
        if (!r.enabled) continue;
        std::string kw = r.caseSensitive ? r.keyword : toLower(r.keyword);
        size_t pos = 0;
        while ((pos = (r.caseSensitive ? text.find(kw, pos) : lower.find(kw, pos))) != std::string::npos) {
            if (r.wholeWord) {
                bool before = pos == 0 || text[pos-1] == ' ';
                bool after = pos + kw.size() >= text.size() || text[pos + kw.size()] == ' ';
                if (before && after) return true;
            } else {
                return true;
            }
            pos++;
        }
    }
    return false;
}

std::string applyHighlights(const std::string& text, const std::vector<HighlightRule>& rules) {
    std::string result = text;
    std::string lower = toLower(text);
    for (const auto& r : rules) {
        if (!r.enabled) continue;
        std::string kw = r.caseSensitive ? r.keyword : toLower(r.keyword);
        size_t pos = 0;
        while ((pos = (r.caseSensitive ? result.find(kw, pos) : toLower(result).find(kw, pos))) != std::string::npos) {
            std::string color = r.color.empty() ? "#FF5722" : r.color;
            result.insert(pos, "<font color=\"" + color + "\">");
            pos += 19 + color.size();
            result.insert(pos + kw.size(), "</font>");
            pos += kw.size() + 7;
        }
    }
    return result;
}

std::string buildHighlightPushRule(const std::vector<HighlightRule>& rules, const std::string& ruleId) {
    std::ostringstream os;
    os << R"({"rule_id":")" << ruleId << R"(",)";
    os << R"("actions":["notify",{"set_tweak":"highlight","value":true}],)";
    os << R"("conditions":[{"kind":"contains_display_name"}],)" ;
    os << R"("pattern":")";
    for (size_t i = 0; i < rules.size(); i++) {
        if (i > 0) os << "|";
        os << rules[i].keyword;
    }
    os << R"("})";
    return os.str();
}

std::vector<HighlightRule> parseHighlightRules(const std::string& json) {
    std::vector<HighlightRule> rules;
    auto patPos = json.find("\"pattern\":\"");
    if (patPos == std::string::npos) return rules;
    patPos += 11;
    auto patEnd = json.find('"', patPos);
    if (patEnd == std::string::npos) return rules;
    std::string pattern = json.substr(patPos, patEnd - patPos);
    
    size_t pos = 0;
    while (pos < pattern.size()) {
        auto bar = pattern.find('|', pos);
        std::string kw = pattern.substr(pos, bar - pos);
        if (!kw.empty()) rules.push_back({kw, "#FF5722", false, false, true});
        if (bar == std::string::npos) break;
        pos = bar + 1;
    }
    return rules;
}

std::string formatHighlightedNotification(const std::string& senderName,
                                            const std::string& body, bool isHighlight) {
    std::ostringstream os;
    os << senderName;
    if (isHighlight) os << " (highlight)";
    os << ": " << body;
    return os.str();
}

} // namespace progressive
