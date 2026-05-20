#include "progressive/login_utils.hpp"
#include "progressive/json_parser.hpp"
#include "progressive/url_tools.hpp"
#include <sstream>
#include <regex>
#include <algorithm>
#include <random>
#include <sstream>

namespace progressive {

std::vector<LoginFlow> parseLoginFlows(const std::string& apiResponseJson) {
    std::vector<LoginFlow> flows;

    // Parse "flows": [{"type": "m.login.password"}, ...]
    size_t pos = 0;
    while (true) {
        pos = apiResponseJson.find("\"type\"", pos);
        if (pos == std::string::npos) break;

        auto objStart = apiResponseJson.rfind('{', pos);
        if (objStart == std::string::npos) break;

        int depth = 0;
        auto objEnd = objStart;
        while (objEnd < apiResponseJson.size()) {
            if (apiResponseJson[objEnd] == '{') ++depth;
            else if (apiResponseJson[objEnd] == '}') --depth;
            if (depth == 0) break;
            ++objEnd;
        }
        if (objEnd >= apiResponseJson.size()) break;

        std::string obj = apiResponseJson.substr(objStart, objEnd - objStart + 1);

        LoginFlow flow;
        flow.type = parseJsonStringValue(obj, "type");
        flow.description = flow.type;
        if (!flow.type.empty()) flows.push_back(flow);
        pos = objEnd + 1;
    }

    return flows;
}

std::string buildUserIdentifier(const std::string& userId) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    if (userId.find('@') != std::string::npos) {
        return R"({"type": "m.id.user", "user": ")" + esc(userId) + R"("})";
    }
    // Phone or email
    if (userId.find('@') != std::string::npos && userId.find(":+") == std::string::npos) {
        return R"({"type": "m.id.thirdparty", "medium": "email", "address": ")" + esc(userId) + R"("})";
    }
    return R"({"type": "m.id.user", "user": ")" + esc(userId) + R"("})";
}

std::string buildLoginBody(const LoginParams& params) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    if (!params.password.empty()) {
        json << R"("type": "m.login.password",)";
        json << R"("identifier": )" << buildUserIdentifier(params.userId) << ",";
        json << R"("password": ")" << esc(params.password) << R"(")";
    } else if (!params.token.empty()) {
        json << R"("type": "m.login.token",)";
        json << R"("token": ")" << esc(params.token) << R"(")";
    }
    if (!params.deviceName.empty())
        json << R"(,"initial_device_display_name": ")" << esc(params.deviceName) << R"(")";
    if (!params.deviceId.empty())
        json << R"(,"device_id": ")" << esc(params.deviceId) << R"(")";
    if (params.refreshToken)
        json << R"(,"refresh_token": true)";
    json << "}";
    return json.str();
}

bool isValidLoginCredentials(const std::string& userId, const std::string& password) {
    return !userId.empty() && userId.size() >= 3 && !password.empty() && password.size() >= 1;
}

LoginResult parseLoginResponse(const std::string& responseJson, int httpStatus) {
    LoginResult result;
    result.httpStatus = httpStatus;

    if (httpStatus != 200) {
        result.errorCode = parseJsonStringValue(responseJson, "errcode");
        result.errorMessage = parseJsonStringValue(responseJson, "error");
        if (result.errorMessage.empty()) result.errorMessage = "Server returned " + std::to_string(httpStatus);
        return result;
    }

    result.userId = parseJsonStringValue(responseJson, "user_id");
    result.accessToken = parseJsonStringValue(responseJson, "access_token");
    result.refreshToken = parseJsonStringValue(responseJson, "refresh_token");
    result.deviceId = parseJsonStringValue(responseJson, "device_id");
    result.homeServer = parseJsonStringValue(responseJson, "home_server");
    result.success = !result.accessToken.empty();

    return result;
}

std::string buildRefreshBody(const std::string& refreshToken) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    return R"({"refresh_token": ")" + esc(refreshToken) + R"("})";
}

bool isRateLimited(const std::string& responseJson, int httpStatus) {
    if (httpStatus == 429) return true;
    auto errcode = parseJsonStringValue(responseJson, "errcode");
    return errcode == "M_LIMIT_EXCEEDED";
}

int getRateLimitRetrySeconds(const std::string& responseJson) {
    auto retryMs = parseJsonStringValue(responseJson, "retry_after_ms");
    if (!retryMs.empty()) return std::stoi(retryMs) / 1000;
    return 5; // default 5 seconds
}

std::string generateDeviceName(const std::string& model, const std::string& osVersion) {
    if (!model.empty() && !osVersion.empty()) return model + " (" + osVersion + ")";
    if (!model.empty()) return model;
    return "Progressive Chat";
}

std::string generateDeviceId() {
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 51);
    std::string id(10, 'A');
    for (int i = 0; i < 10; ++i) id[i] = chars[dis(gen)];
    return id;
}

std::string parseDeviceName(const std::string& userAgent) {
    // Extract meaningful name from UA
    auto pos = userAgent.find("Progressive");
    if (pos != std::string::npos) return userAgent.substr(pos);
    pos = userAgent.find("Element");
    if (pos != std::string::npos) return userAgent.substr(pos);
    return userAgent.substr(0, std::min(size_t(50), userAgent.size()));
}

WellKnownResult parseWellKnown(const std::string& responseJson) {
    WellKnownResult result;

    auto homeserver = parseJsonStringValue(responseJson, "base_url");
    if (!homeserver.empty()) {
        result.homeServerBaseUrl = homeserver;
        result.valid = true;
        result.isWellKnown = true;
    }

    auto identity = parseJsonStringValue(responseJson, "id_base_url");
    if (!identity.empty()) {
        result.identityServerBaseUrl = identity;
    }

    return result;
}

bool needsWellKnownDiscovery(const std::string& homeserverUrl) {
    return homeserverUrl.find("matrix.org") == std::string::npos &&
           homeserverUrl.find("://matrix") == std::string::npos;
}

std::string buildWellKnownUrl(const std::string& domain) {
    return "https://" + domain + "/.well-known/matrix/client";
}

} // namespace progressive
