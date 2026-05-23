#pragma once
#include <string>

std::string parseVisibility(const std::string& json);
std::string isVisibleRoom(const std::string& json);
std::string buildVisibilityEvent(const std::string& json);
