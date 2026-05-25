#include "progressive/media_thumbnailer.hpp"
#include <sstream>

std::string generateThumbnail(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"generateThumbnail"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getThumbnailSize(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getThumbnailSize"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isAnimated(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isAnimated"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMimeForThumb(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMimeForThumb"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
