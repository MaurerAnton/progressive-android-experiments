#pragma once
#include <string>

std::string parseJoinRules(const std::string& json);
std::string isPublicRoom(const std::string& json);
std::string buildJoinRulesEvent(const std::string& json);
