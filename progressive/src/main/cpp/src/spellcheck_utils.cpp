#include "progressive/spellcheck_utils.hpp"
#include <sstream>

std::string loadDictionary(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"loadDictionary"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string checkSpelling(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkSpelling"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getSuggestions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSuggestions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string tokenizeForSpelling(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"tokenizeForSpelling"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
