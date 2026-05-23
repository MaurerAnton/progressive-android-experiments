#pragma once
#include <string>
#include <cstdint>

std::string parsePollStart(const std::string& json);
std::string parsePollEnd(const std::string& json);
std::string buildPollResponse(const std::string& json);
std::string parsePollResults(const std::string& json);
std::string canVotePoll(const std::string& json);
