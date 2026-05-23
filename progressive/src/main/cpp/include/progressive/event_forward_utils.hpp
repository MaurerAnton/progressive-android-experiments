#pragma once
#include <string>
#include <vector>
namespace progressive {
std::string buildForwardRequest(const std::vector<std::string>& roomIds);
std::string buildForwardContent(const std::string& eventId, const std::string& roomId);
}
