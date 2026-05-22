#pragma once
#include <string>
#include <vector>

namespace progressive {

// Build event filter JSON for /sync requests
std::string buildEventFilter(const std::vector<std::string>& types, const std::vector<std::string>& notTypes,
                               const std::vector<std::string>& senders, const std::vector<std::string>& notSenders,
                               int limit = 20);

// Build room-specific filter
std::string buildRoomFilter(const std::vector<std::string>& rooms, const std::string& eventFilter);

} // namespace progressive
