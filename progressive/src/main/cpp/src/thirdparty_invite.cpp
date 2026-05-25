#include "progressive/thirdparty_invite.hpp"
#include <sstream>

std::string parseThirdpartyInvite(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseThirdpartyInvite"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string acceptThirdparty(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"acceptThirdparty"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string rejectThirdparty(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"rejectThirdparty"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isValidThirdparty(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isValidThirdparty"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
