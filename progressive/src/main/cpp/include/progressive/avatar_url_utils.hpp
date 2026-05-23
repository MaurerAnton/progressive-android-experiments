#pragma once
#include <string>

std::string parseAvatarUrl(const std::string& json);
std::string buildAvatarUpdateEvent(const std::string& json);
