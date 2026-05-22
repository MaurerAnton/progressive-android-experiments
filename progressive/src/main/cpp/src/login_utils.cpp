#include "progressive/login_utils.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

std::vector<LoginFlow> parseLoginFlows(const std::string& json) {
    std::vector<LoginFlow> flows;
    size_t pos = 0;
    while (true) {
        auto typePos = json.find("\"type\":\"", pos);
        if (typePos == std::string::npos) break;
        typePos += 8;
        auto typeEnd = json.find('"', typePos);
        if (typeEnd == std::string::npos) break;
        
        LoginFlow f;
        f.type = json.substr(typePos, typeEnd - typePos);
        flows.push_back(f);
        pos = typeEnd + 1;
    }
    return flows;
}

std::string buildLoginRequest(const LoginParams& p) {
    std::ostringstream os;
    os << "{";
    os << R"("type":")" << p.type << R"(",)";
    os << R"("identifier":{")";
    os << R"("type":")" << p.identifierType << R"(",)";
    os << R"("user":")" << p.identifier << R"(")";
    os << "}";
    if (!p.password.empty()) os << R"(,"password":")" << p.password << R"(")";
    if (!p.token.empty()) os << R"(,"token":")" << p.token << R"(")";
    if (!p.deviceId.empty()) os << R"(,"device_id":")" << p.deviceId << R"(")";
    if (!p.initialDeviceName.empty()) os << R"(,"initial_device_display_name":")" << p.initialDeviceName << R"(")";
    os << "}";
    return os.str();
}

std::string parseLoginResponse(const std::string& json) {
    std::ostringstream os;
    os << "{";
    // Extract key fields
    auto accessField = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    os << R"("access_token":")" << accessField("access_token") << R"(")";
    os << R"(,"device_id":")" << accessField("device_id") << R"(")";
    os << R"(,"user_id":")" << accessField("user_id") << R"(")";
    os << R"(,"home_server":")" << accessField("home_server") << R"(")";
    os << "}";
    return os.str();
}

bool supportsFlow(const std::vector<LoginFlow>& flows, const std::string& flowType) {
    return std::any_of(flows.begin(), flows.end(),
        [&](const LoginFlow& f) { return f.type == flowType; });
}

bool isValidUserId(const std::string& id) {
    return id.size() > 1 && id[0] == '@' && id.find(':') != std::string::npos;
}

bool isValidEmail(const std::string& email) {
    auto at = email.find('@');
    return at != std::string::npos && at > 0 && at < email.size() - 1 &&
           email.find('.', at) != std::string::npos;
}

bool isValidPhoneNumber(const std::string& phone) {
    return phone.size() >= 7 && std::all_of(phone.begin(), phone.end(),
        [](char c) { return isdigit(c) || c == '+' || c == '-' || c == ' ' || c == '(' || c == ')'; });
}

} // namespace progressive
