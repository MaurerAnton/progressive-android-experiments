#include "progressive/verification_request_utils.hpp"
#include <sstream>

std::string parseVerificationRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseVerificationRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string acceptRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"acceptRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string declineRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"declineRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRequestStatus(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRequestStatus"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
