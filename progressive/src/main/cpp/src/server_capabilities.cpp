#include "progressive/server_capabilities.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

std::vector<std::string> parseServerVersions(const std::string& json) {
    std::vector<std::string> versions;
    auto pos = json.find("\"versions\"");
    if (pos == std::string::npos) return versions;
    auto arr = json.find('[', pos);
    auto arrEnd = json.find(']', arr);
    if (arr == std::string::npos || arrEnd == std::string::npos) return versions;
    
    std::string arrStr = json.substr(arr + 1, arrEnd - arr - 1);
    size_t s = 0;
    while (s < arrStr.size()) {
        auto q1 = arrStr.find('"', s);
        if (q1 == std::string::npos) break;
        auto q2 = arrStr.find('"', q1 + 1);
        if (q2 == std::string::npos) break;
        versions.push_back(arrStr.substr(q1 + 1, q2 - q1 - 1));
        s = q2 + 1;
    }
    return versions;
}

ServerCapabilities parseCapabilities(const std::string& json) {
    ServerCapabilities c;
    c.supportsThreads = json.find("\"m.thread\"") != std::string::npos;
    c.supportsPolls = json.find("\"m.poll\"") != std::string::npos;
    c.supportsLocation = json.find("\"m.location\"") != std::string::npos;
    c.supportsSSO = json.find("\"m.login.sso\"") != std::string::npos;
    c.supportsPasswordLogin = json.find("\"m.login.password\"") != std::string::npos;
    c.supportsTokenRefresh = json.find("\"m.refresh_token\"") != std::string::npos;
    
    auto roomVers = json.find("\"m.room_versions\"");
    if (roomVers != std::string::npos) {
        auto avail = json.find("\"available\"", roomVers);
        if (avail != std::string::npos) {
            c.roomVersions = parseServerVersions(json.substr(avail));
        }
    }
    return c;
}

bool supportsLoginFlow(const ServerCapabilities& caps, const std::string& flow) {
    return std::find(caps.loginFlows.begin(), caps.loginFlows.end(), flow) != caps.loginFlows.end();
}

std::string getDefaultRoomVersion(const ServerCapabilities& caps) {
    if (caps.roomVersions.empty()) return "10";
    for (const auto& v : caps.roomVersions) {
        if (v == "11" || v == "10") return v;
    }
    return caps.roomVersions.back();
}

std::string formatCapabilities(const ServerCapabilities& caps) {
    std::ostringstream os;
    os << "Server supports: ";
    if (caps.supportsThreads) os << "Threads ";
    if (caps.supportsPolls) os << "Polls ";
    if (caps.supportsLocation) os << "Location ";
    if (caps.supportsSSO) os << "SSO ";
    return os.str();
}

} // namespace progressive
