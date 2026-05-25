#include "progressive/room_autocomplete.hpp"
#include <sstream>

std::string getUserSuggestions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getUserSuggestions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getEmojiSuggestions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getEmojiSuggestions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRoomSuggestions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRoomSuggestions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string rankSuggestions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"rankSuggestions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
