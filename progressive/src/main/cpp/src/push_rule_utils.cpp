#include "progressive/push_rule_utils.hpp"
#include <sstream>

std::string parsePushRule(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parsePushRule"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string matchPushRule(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"matchPushRule"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildPushRuleEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildPushRuleEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
