#include "progressive/media_compressor.hpp"
#include <sstream>

std::string compressImage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"compressImage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string compressVideo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"compressVideo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getCompressionRatio(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getCompressionRatio"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string originalSize(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"originalSize"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
