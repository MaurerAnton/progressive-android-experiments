#include "progressive/url_preview_utils.hpp"
#include <sstream>

std::string parseOpenGraph(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseOpenGraph"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractMetadata(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractMetadata"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildPreviewSnippet(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildPreviewSnippet"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
