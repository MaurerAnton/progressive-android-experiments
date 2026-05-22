#pragma once
#include <string>

namespace progressive {

// Generate default device name from device model + OS
std::string generateDeviceName(const std::string& manufacturer, const std::string& model,
                                 const std::string& osName, const std::string& osVersion);

// Parse device name from user agent
std::string parseDeviceNameFromUserAgent(const std::string& userAgent);

// Format device display name with OS info
std::string formatDeviceDisplayName(const std::string& deviceName, const std::string& osInfo,
                                      const std::string& appVersion);

// Check if device name is a default (auto-generated vs custom)
bool isDefaultDeviceName(const std::string& name);

// Truncate device name to max length
std::string truncateDeviceName(const std::string& name, int maxLen = 50);

} // namespace progressive
