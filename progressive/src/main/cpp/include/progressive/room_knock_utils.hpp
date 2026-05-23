#pragma once
#include <string>
#include <vector>
namespace progressive {
std::string buildKnockRequest(const std::string& roomId, const std::vector<std::string>& via={}, const std::string& reason="");
std::string formatKnockResponse(const std::string& json);
}
