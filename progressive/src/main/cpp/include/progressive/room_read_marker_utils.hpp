#pragma once
#include <string>

std::string parseReadMarker(const std::string& json);
std::string updateReadMarker(const std::string& json);
std::string buildReadMarkerEvent(const std::string& json);
