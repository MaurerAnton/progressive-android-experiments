#include "progressive/event_search.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>

namespace progressive {

std::string extractSearchableText(const std::string& json) {
    std::string text;
    auto bodyPos = json.find("\"body\":\"");
    if (bodyPos != std::string::npos) {
        bodyPos += 8;
        auto end = json.find('"', bodyPos);
        if (end != std::string::npos) text = json.substr(bodyPos, end - bodyPos);
    }
    auto fbodyPos = json.find("\"formatted_body\":\"");
    if (fbodyPos != std::string::npos) {
        fbodyPos += 18;
        // Strip HTML tags
        bool inTag = false;
        for (size_t i = fbodyPos; i < json.size(); i++) {
            if (json[i] == '"') break;
            if (json[i] == '<') { inTag = true; continue; }
            if (json[i] == '>') { inTag = false; continue; }
            if (!inTag) text += json[i];
        }
    }
    return text;
}

static std::string toLower(const std::string& s) {
    std::string r; r.reserve(s.size());
    std::transform(s.begin(), s.end(), std::back_inserter(r), ::tolower);
    return r;
}

EventSearchResult searchEvents(const std::vector<SearchableEvent>& events,
                                const std::string& query, bool caseSensitive, int maxResults) {
    auto start = std::chrono::steady_clock::now();
    EventSearchResult result;
    result.query = query;
    
    std::string q = caseSensitive ? query : toLower(query);
    for (const auto& ev : events) {
        std::string body = caseSensitive ? ev.body : toLower(ev.body);
        if (body.find(q) != std::string::npos) {
            result.matches.push_back(ev);
            if ((int)result.matches.size() >= maxResults) break;
        }
    }
    
    auto end = std::chrono::steady_clock::now();
    result.totalMatches = (int)result.matches.size();
    result.searchTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return result;
}

std::string highlightMatches(const std::string& text, const std::string& query, bool caseSensitive) {
    std::string q = caseSensitive ? query : toLower(query);
    std::string t = caseSensitive ? text : toLower(text);
    std::string result;
    size_t pos = 0;
    while (pos < t.size()) {
        auto found = t.find(q, pos);
        if (found == std::string::npos) {
            result += text.substr(pos);
            break;
        }
        result += text.substr(pos, found - pos);
        result += "[match]" + text.substr(found, q.size()) + "[/match]";
        pos = found + q.size();
    }
    return result;
}

bool matchesFilter(const SearchableEvent& event,
                    const std::string& typeFilter,
                    const std::vector<std::string>& senderFilter,
                    int64_t afterMs, int64_t beforeMs) {
    if (!typeFilter.empty() && event.type != typeFilter) return false;
    if (!senderFilter.empty()) {
        bool found = false;
        for (const auto& s : senderFilter)
            if (event.senderId == s) { found = true; break; }
        if (!found) return false;
    }
    if (afterMs > 0 && event.timestampMs < afterMs) return false;
    if (beforeMs > 0 && event.timestampMs > beforeMs) return false;
    return true;
}

std::string formatSearchResult(const SearchableEvent& event, const std::string& query) {
    std::ostringstream os;
    os << "[" << event.senderId << "] ";
    auto highlighted = highlightMatches(event.body, query);
    // Strip [match] tags for plain display
    std::string clean;
    for (size_t i = 0; i < highlighted.size(); i++) {
        if (highlighted.substr(i, 7) == "[match]") { i += 6; continue; }
        if (highlighted.substr(i, 8) == "[/match]") { i += 7; continue; }
        clean += highlighted[i];
    }
    os << clean;
    return os.str();
}

} // namespace progressive
