#include "progressive/url_render_utils.hpp"
#include <sstream>

std::string buildUrlPreviewHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildUrlPreviewHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatUrlForDisplay(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatUrlForDisplay"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
