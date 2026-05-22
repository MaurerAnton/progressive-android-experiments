#pragma once
#include <string>
#include <vector>

namespace progressive {

// Build join room request body
std::string buildJoinRequest(const std::vector<std::string>& viaServers = {});

// Build join by alias request
std::string buildJoinByAlias(const std::string& alias, const std::vector<std::string>& via = {});

// Parse join response (room_id)
std::string parseJoinResponse(const std::string& json);

// Format join confirmation
std::string formatJoinConfirmation(const std::string& roomId);

} // namespace progressive
