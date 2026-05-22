#pragma once
#include <string>
#include <vector>

namespace progressive {

struct LoginFlow {
    std::string type;           // "m.login.password", "m.login.sso", etc.
    std::vector<std::string> stages;
};

struct LoginParams {
    std::string type;           // login flow type
    std::string identifier;     // user ID or email/phone
    std::string identifierType; // "m.id.user", "m.id.thirdparty", "m.id.phone"
    std::string password;
    std::string token;          // SSO or other token
    std::string deviceId;
    std::string initialDeviceName;
};

// Parse login flows from /login response
std::vector<LoginFlow> parseLoginFlows(const std::string& json);

// Build login request JSON
std::string buildLoginRequest(const LoginParams& params);

// Parse login response (access_token, device_id, user_id)
std::string parseLoginResponse(const std::string& json);

// Check if a login flow is supported
bool supportsFlow(const std::vector<LoginFlow>& flows, const std::string& flowType);

// Validate identifier format
bool isValidUserId(const std::string& id);
bool isValidEmail(const std::string& email);
bool isValidPhoneNumber(const std::string& phone);

} // namespace progressive
