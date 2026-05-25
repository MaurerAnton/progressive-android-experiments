#include "progressive/olm_utils.hpp"
#include <sstream>

std::string createAccount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"createAccount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string signMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"signMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string verifySignature(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"verifySignature"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string generateKeys(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"generateKeys"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
