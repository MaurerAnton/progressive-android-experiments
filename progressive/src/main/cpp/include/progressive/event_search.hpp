#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct SearchableEvent {
    std::string eventId;
    std::string type;
    std::string senderId;
    std::string body;           // plain text body for search
    int64_t timestampMs = 0;
    int displayIndex = -1;      // position in timeline
};

struct EventSearchResult {
    std::string query;
    std::vector<SearchableEvent> matches;
    int totalMatches = 0;
    int64_t searchTimeMs = 0;
};

// Extract searchable text from an event JSON
std::string extractSearchableText(const std::string& eventJson);

// Search events by keyword
EventSearchResult searchEvents(const std::vector<SearchableEvent>& events,
                                const std::string& query,
                                bool caseSensitive = false,
                                int maxResults = 50);

// Highlight search matches in text (wraps in [match]...[/match])
std::string highlightMatches(const std::string& text, const std::string& query,
                              bool caseSensitive = false);

// Check if an event matches search filters (type, sender, date range)
bool matchesFilter(const SearchableEvent& event,
                   const std::string& typeFilter = "",
                   const std::vector<std::string>& senderFilter = {},
                   int64_t afterMs = 0, int64_t beforeMs = 0);

// Format search results for display
std::string formatSearchResult(const SearchableEvent& event, const std::string& query);

} // namespace progressive
