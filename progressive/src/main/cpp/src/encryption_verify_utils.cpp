#include "progressive/encryption_verify_utils.hpp"
#include <sstream>

std::string verifyDeviceKey(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"verifyDeviceKey"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string verifyCrossSign(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"verifyCrossSign"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string checkKeyTrust(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkKeyTrust"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getTrustLevel(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getTrustLevel"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
