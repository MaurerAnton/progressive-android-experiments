#pragma once
#include <string>

std::string parseGuestAccess(const std::string& json);
std::string isGuestAllowed(const std::string& json);
std::string buildGuestAccessEvent(const std::string& json);
