#pragma once
#include <string>
namespace progressive {
std::string buildDirectMessageFlag(const std::string& userId, const std::string& roomId);
bool isDirectMessage(const std::string& accountDataJson, const std::string& roomId);
}
