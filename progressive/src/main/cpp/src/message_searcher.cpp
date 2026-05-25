#include "progressive/message_searcher.hpp"
#include <sstream>

std::string searchMessages(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"searchMessages"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string highlightMatches(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"highlightMatches"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string tokenizeContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"tokenizeContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildSearchIndex(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSearchIndex"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
