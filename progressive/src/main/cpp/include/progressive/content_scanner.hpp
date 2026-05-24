#pragma once
#include <string>
#include <cstdint>

std::string scanUrl(const std::string& json);
std::string isPhishing(const std::string& json);
std::string scanAttachment(const std::string& json);
std::string isMalware(const std::string& json);
std::string getScanResult(const std::string& json);
