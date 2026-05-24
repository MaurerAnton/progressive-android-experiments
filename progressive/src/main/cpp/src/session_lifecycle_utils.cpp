#include "progressive/session_lifecycle_utils.hpp"
namespace progressive {
std::string buildLogoutRequest(){return"{}";}
std::string buildLogoutAllRequest(){return R"({"logout_devices":true})";}
std::string parseSessionState(const std::string& json){return json.empty()?"active":"unknown";}
bool isSessionSoftLoggedOut(const std::string& json){return json.find("soft_logout")!=std::string::npos||json.find("M_UNKNOWN_TOKEN")!=std::string::npos;}
}
