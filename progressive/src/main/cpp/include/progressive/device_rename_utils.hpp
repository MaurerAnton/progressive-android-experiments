#pragma once
#include <string>
namespace progressive {
std::string buildDeviceRenameRequest(const std::string& deviceId, const std::string& newName);
std::string parseDeviceRenameResponse(const std::string& json);
}
