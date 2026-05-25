#include "progressive/gossip_manager.hpp"
#include <sstream>

std::string gossipKeyRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"gossipKeyRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string receiveGossip(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"receiveGossip"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getPendingGossips(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getPendingGossips"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string cancelGossip(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"cancelGossip"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
