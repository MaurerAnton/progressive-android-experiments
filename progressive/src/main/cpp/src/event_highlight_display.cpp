#include "progressive/event_highlight_display.hpp"
#include <sstream>

std::string getHighlightColor(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getHighlightColor"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatHighlightBadge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatHighlightBadge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getHighlightSoundHint(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getHighlightSoundHint"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
