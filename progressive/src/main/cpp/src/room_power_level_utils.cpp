#include "progressive/room_power_level_utils.hpp"
#include <sstream>

namespace progressive {

PowerLevelInfo parsePowerLevelInfo(const std::string& json) {
    PowerLevelInfo pl;
    auto extractInt = [&](const std::string& key) {
        auto p = json.find("\"" + key + "\":"); if (p == std::string::npos) return;
        p += key.size() + 2; while (p < json.size() && json[p] == ' ') p++;
        try { 
            if (key == "users_default") pl.usersDefault = std::stoi(json.substr(p));
            else if (key == "events_default") pl.eventsDefault = std::stoi(json.substr(p));
            else if (key == "state_default") pl.stateDefault = std::stoi(json.substr(p));
            else if (key == "ban") pl.ban = std::stoi(json.substr(p));
            else if (key == "kick") pl.kick = std::stoi(json.substr(p));
            else if (key == "redact") pl.redact = std::stoi(json.substr(p));
            else if (key == "invite") pl.invite = std::stoi(json.substr(p));
        } catch(...) {}
    };
    extractInt("users_default"); extractInt("events_default"); extractInt("state_default");
    extractInt("ban"); extractInt("kick"); extractInt("redact"); extractInt("invite");
    return pl;
}
int getUserLevel(const PowerLevelInfo& pl, const std::string& uid) {
    auto it = pl.users.find(uid); return it != pl.users.end() ? it->second : pl.usersDefault;
}
bool canUserBan(const PowerLevelInfo& pl, const std::string& uid) { return getUserLevel(pl, uid) >= pl.ban; }
bool canUserKick(const PowerLevelInfo& pl, const std::string& uid) { return getUserLevel(pl, uid) >= pl.kick; }
bool canUserRedact(const PowerLevelInfo& pl, const std::string& uid) { return getUserLevel(pl, uid) >= pl.redact; }
bool canUserInvite(const PowerLevelInfo& pl, const std::string& uid) { return getUserLevel(pl, uid) >= pl.invite; }
bool canUserChangeState(const PowerLevelInfo& pl, const std::string& uid) { return getUserLevel(pl, uid) >= pl.stateDefault; }
bool isUserAdmin(const PowerLevelInfo& pl, const std::string& uid) { return getUserLevel(pl, uid) >= 100; }
std::string formatPowerLevel(int l) {
    if (l >= 100) return "Admin (100)"; if (l >= 50) return "Moderator (50)"; return "User (" + std::to_string(l) + ")";
}
std::string formatUserRole(const PowerLevelInfo& pl, const std::string& uid) {
    return formatPowerLevel(getUserLevel(pl, uid));
}
} // namespace progressive
