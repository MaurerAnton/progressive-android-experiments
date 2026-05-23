#pragma once
#include <string>
#include <cstdint>

std::string parseAccountData(const std::string& json);
std::string buildAccountDataEvent(const std::string& json);
std::string mergeAccountData(const std::string& json);
std::string getDirectMessageMap(const std::string& json);
