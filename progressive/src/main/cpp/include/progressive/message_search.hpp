#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct MessageSearchHit {
    std::string eventId;
    std::string roomId;
    std::string senderId;
    std::string body;
    int64_t timestampMs = 0;
    std::string context;        // surrounding text
    double score = 0.0;
};

struct MessageSearchResult {
    std::string query;
    std::vector<MessageSearchHit> hits;
    int totalResults = 0;
    std::string nextBatch;
};

// Build message search request body
std::string buildSearchRequest(const std::string& query, const std::vector<std::string>& roomIds = {},
                                 const std::string& filter = "", int limit = 20);

// Parse search response
MessageSearchResult parseSearchResponse(const std::string& json);

// Extract context around a search match
std::string extractSearchContext(const std::string& body, const std::string& query, int contextChars = 80);

// Rank results by relevance
std::vector<MessageSearchHit> rankSearchResults(const std::vector<MessageSearchHit>& hits,
                                                  const std::string& query);

} // namespace progressive
