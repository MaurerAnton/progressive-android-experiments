#pragma once
#include <string>
#include <unordered_map>

namespace progressive {

struct PowerLevelInfo {
    int usersDefault = 0;
    int eventsDefault = 0;
    int stateDefault = 50;
    int ban = 50;
    int kick = 50;
    int redact = 50;
    int invite = 0;
    std::unordered_map<std::string, int> users;
};

// Parse full power levels from state event
PowerLevelInfo parsePowerLevelInfo(const std::string& json);

// Check specific permissions
int getUserLevel(const PowerLevelInfo& pl, const std::string& userId);
bool canUserBan(const PowerLevelInfo& pl, const std::string& userId);
bool canUserKick(const PowerLevelInfo& pl, const std::string& userId);
bool canUserRedact(const PowerLevelInfo& pl, const std::string& userId);
bool canUserInvite(const PowerLevelInfo& pl, const std::string& userId);
bool canUserChangeState(const PowerLevelInfo& pl, const std::string& userId);
bool isUserAdmin(const PowerLevelInfo& pl, const std::string& userId);

// Format power level for display
std::string formatPowerLevel(int level);
std::string formatUserRole(const PowerLevelInfo& pl, const std::string& userId);

} // namespace progressive
