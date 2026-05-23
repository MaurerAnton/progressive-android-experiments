#pragma once
#include <string>

std::string parseSuggestedRole(const std::string& json);
std::string getDefaultRole(const std::string& json);
std::string buildRoleSuggestion(const std::string& json);
