#pragma once
#include <string>
#include <cstdint>

std::string sortTimelineEvents(const std::string& json);
std::string deduplicateEvents(const std::string& json);
std::string mergeLocalAndRemoteEvents(const std::string& json);
std::string applyGap(const std::string& json);
std::string calculateReadMarker(const std::string& json);
