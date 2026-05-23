#pragma once
#include <string>

std::string parseParentRoomId(const std::string& json);
std::string getParentSpaces(const std::string& json);
std::string buildParentEvent(const std::string& json);
