#include "progressive/key_verification_utils.hpp"
#include <sstream>

std::string parseVerificationStart(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseVerificationStart"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildVerificationAccept(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildVerificationAccept"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseKeyVerificationMac(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseKeyVerificationMac"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string validateSasEmoji(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"validateSasEmoji"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
