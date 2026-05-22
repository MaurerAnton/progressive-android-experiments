#pragma once
#include <string>
#include <vector>

namespace progressive {

// Check if message body contains user's display name (highlight)
bool containsDisplayName(const std::string& body, const std::string& displayName);

// Check if message matches highlight rules (custom keywords)
bool matchesHighlightRules(const std::string& body, const std::vector<std::string>& keywords);

// Parse highlight count from notification
int parseHighlightCount(const std::string& notifJson);

} // namespace progressive
