#include "progressive/link_previewer.hpp"
#include <sstream>

std::string parseLinkMetadata(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseLinkMetadata"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getOgpTags(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getOgpTags"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isPreviewable(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isPreviewable"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatPreviewHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatPreviewHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
