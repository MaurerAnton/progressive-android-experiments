#include "progressive/secret_sharing.hpp"
#include <sstream>

std::string shareSecret(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"shareSecret"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string requestSecret(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"requestSecret"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string cancelSecretRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"cancelSecretRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseSecretEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseSecretEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
