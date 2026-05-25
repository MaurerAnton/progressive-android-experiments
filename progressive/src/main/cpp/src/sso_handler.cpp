#include "progressive/sso_handler.hpp"
#include <sstream>

std::string parseSsoRedirect(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseSsoRedirect"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildSsoUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSsoUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractTokenFromUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractTokenFromUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getProviderName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getProviderName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
