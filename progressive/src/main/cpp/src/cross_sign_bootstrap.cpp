#include "progressive/cross_sign_bootstrap.hpp"
#include <sstream>

std::string bootstrapCrossSign(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"bootstrapCrossSign"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getBootstrapStatus(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getBootstrapStatus"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isBootstrapped(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isBootstrapped"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string resetCrossSign(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"resetCrossSign"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
