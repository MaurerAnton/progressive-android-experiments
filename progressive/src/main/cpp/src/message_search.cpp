#include "progressive/message_search.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

std::string buildSearchRequest(const std::string& query, const std::vector<std::string>& roomIds,
                                 const std::string& filter, int limit) {
    std::ostringstream os;
    os << R"({"search_categories":{"room_events":{)";
    os << R"("search_term":")" << query << R"(")";
    if (!filter.empty()) os << R"(,"filter":)" << filter;
    os << R"(,"order_by":"recent")";
    os << R"(,"include_state":false)";
    os << "}";
    if (!roomIds.empty()) {
        os << R"(,"filter":{"rooms":[)";
        for (size_t i = 0; i < roomIds.size(); i++) {
            if (i > 0) os << ",";
            os << R"(")" << roomIds[i] << R"(")";
        }
        os << "]}";
    }
    os << R"(,"limit":)" << limit;
    os << "}}";
    return os.str();
}

MessageSearchResult parseSearchResponse(const std::string& json) {
    MessageSearchResult r;
    r.totalResults = 0;
    
    size_t pos = 0;
    while (pos < json.size()) {
        auto evtPos = json.find("\"event_id\":\"", pos);
        if (evtPos == std::string::npos) break;
        evtPos += 12;
        auto evtEnd = json.find('"', evtPos);
        if (evtEnd == std::string::npos) break;
        
        MessageSearchHit h;
        h.eventId = json.substr(evtPos, evtEnd - evtPos);
        r.hits.push_back(h);
        r.totalResults++;
        pos = evtEnd + 1;
    }
    return r;
}

std::string extractSearchContext(const std::string& body, const std::string& query, int contextChars) {
    auto lower = body;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    auto q = query;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);
    
    auto pos = lower.find(q);
    if (pos == std::string::npos) return body.substr(0, contextChars);
    
    int start = (int)pos - contextChars / 2;
    if (start < 0) start = 0;
    int end = (int)pos + contextChars / 2;
    if (end > (int)body.size()) end = (int)body.size();
    
    std::string ctx = body.substr(start, end - start);
    if (start > 0) ctx = "..." + ctx;
    if (end < (int)body.size()) ctx += "...";
    return ctx;
}

std::vector<MessageSearchHit> rankSearchResults(const std::vector<MessageSearchHit>& hits,
                                                  const std::string& query) {
    auto ranked = hits;
    for (auto& h : ranked) {
        auto lower = h.body;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        auto q = query;
        std::transform(q.begin(), q.end(), q.begin(), ::tolower);
        if (lower.find(q) == 0) h.score = 1.0;
        else if (lower.find(q) != std::string::npos) h.score = 0.5;
        else h.score = 0.1;
    }
    std::sort(ranked.begin(), ranked.end(), [](auto& a, auto& b) { return a.score > b.score; });
    return ranked;
}

} // namespace progressive
