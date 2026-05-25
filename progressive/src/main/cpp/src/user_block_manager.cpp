#include "progressive/user_block_manager.hpp"
#include <sstream>

std::string blockUser(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"blockUser"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string unblockUser(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"unblockUser"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isBlocked(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isBlocked"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getBlockList(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getBlockList"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
