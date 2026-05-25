#include "progressive/device_rename_utils.hpp"
#include <sstream>

std::string buildDeviceRenameRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildDeviceRenameRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseDeviceRenameResponse(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseDeviceRenameResponse"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
