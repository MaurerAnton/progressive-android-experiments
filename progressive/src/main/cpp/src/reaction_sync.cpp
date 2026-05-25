#include "progressive/reaction_sync.hpp"
#include <sstream>

std::string syncReactions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"syncReactions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string aggregateReactions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"aggregateReactions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMyReaction(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMyReaction"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string removeReaction(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"removeReaction"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
