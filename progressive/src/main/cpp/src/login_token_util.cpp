#include "progressive/login_token_util.hpp"
#include <sstream>

std::string parseLoginToken(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseLoginToken"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string validateTokenExpiry(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"validateTokenExpiry"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildTokenLogin(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildTokenLogin"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getTokenUser(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getTokenUser"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
