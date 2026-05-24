#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string isOnline(const std::string& json);
std::string getNetworkType(const std::string& json);
std::string getSignalStrength(const std::string& json);
std::string monitorChanges(const std::string& json);
std::string lastOnlineTimestamp(const std::string& json);
