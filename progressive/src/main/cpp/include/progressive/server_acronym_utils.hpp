#pragma once
#include <string>
namespace progressive {
std::string getServerAcronym(const std::string& serverName);
std::string formatServerName(const std::string& serverName);
bool isDefaultServer(const std::string& serverName);
}
