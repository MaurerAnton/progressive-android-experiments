#pragma once
#include <string>

namespace progressive {

// Extract state key from event JSON
std::string parseStateKey(const std::string& json);

// Check if event is a state event
bool isStateEvent(const std::string& json);

// Build state event content with state key
std::string buildStateEvent(const std::string& type, const std::string& stateKey,
                              const std::string& content);

// Format state event for timeline display
std::string formatStateEvent(const std::string& type, const std::string& stateKey,
                               const std::string& senderName);

} // namespace progressive
