#include "progressive/key_share.hpp"
#include <sstream>

std::string requestKeyShare(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"requestKeyShare"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string processKeyShare(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"processKeyShare"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isKeyKnown(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isKeyKnown"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMissingSessions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMissingSessions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
