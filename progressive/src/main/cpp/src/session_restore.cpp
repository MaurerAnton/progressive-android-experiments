#include "progressive/session_restore.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

std::string serializeSession(const SessionData& d) {
    std::ostringstream os;
    os << "{";
    os << R"("user_id":")" << d.userId << R"(",)";
    os << R"("access_token":")" << d.accessToken << R"(",)";
    os << R"("device_id":")" << d.deviceId << R"(",)";
    os << R"("home_server":")" << d.homeServer << R"(",)";
    os << R"("last_active_ms":)" << d.lastActiveMs;
    if (!d.refreshToken.empty()) os << R"(,"refresh_token":")" << d.refreshToken << R"(")";
    os << "}";
    return os.str();
}

SessionData deserializeSession(const std::string& json) {
    SessionData d;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    d.userId = extract("user_id");
    d.accessToken = extract("access_token");
    d.deviceId = extract("device_id");
    d.homeServer = extract("home_server");
    d.refreshToken = extract("refresh_token");
    return d;
}

bool isSessionExpired(const std::string& errorResponse) {
    return errorResponse.find("M_UNKNOWN_TOKEN") != std::string::npos ||
           errorResponse.find("M_MISSING_TOKEN") != std::string::npos ||
           errorResponse.find("soft_logout") != std::string::npos;
}

SessionData extractSessionFromLogin(const std::string& json) {
    SessionData d;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    d.userId = extract("user_id");
    d.accessToken = extract("access_token");
    d.deviceId = extract("device_id");
    d.homeServer = extract("home_server");
    d.lastActiveMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return d;
}

std::string buildSessionKey(const std::string& userId, const std::string& deviceId) {
    return userId + "|" + deviceId;
}

std::string formatSessionInfo(const SessionData& d) {
    std::ostringstream os;
    os << d.userId << " @ " << d.homeServer;
    if (!d.deviceId.empty()) os << " [" << d.deviceId << "]";
    return os.str();
}

} // namespace progressive
