#include "progressive/media_download.hpp"
#include <sstream>

std::string getDownloadUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDownloadUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isDownloaded(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isDownloaded"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDownloadPath(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDownloadPath"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDownloadProgress(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDownloadProgress"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
