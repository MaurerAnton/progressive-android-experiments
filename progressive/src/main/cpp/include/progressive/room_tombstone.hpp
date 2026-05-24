#pragma once
#include <string>
#include <cstdint>

std::string parseTombstone(const std::string& json);
std::string isTombstoned(const std::string& json);
std::string getReplacementRoom(const std::string& json);
std::string buildTombstoneEvent(const std::string& json);
std::string formatTombstoneNotice(const std::string& json);
