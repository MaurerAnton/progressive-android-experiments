#pragma once
#include <string>
namespace progressive {
bool canUserReadHistory(const std::string& visibility, const std::string& membership, bool wasInvited=false);
std::string getDefaultHistoryVisibility();
std::string formatHistoryVisibility(const std::string& vis);
}
