#pragma once
#include <string>

std::string parseContentType(const std::string& json);
std::string extractTextBody(const std::string& json);
std::string extractFormattedBody(const std::string& json);
std::string extractMediaUrl(const std::string& json);
