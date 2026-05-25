#include "progressive/message_hasher.hpp"
#include <sstream>

std::string hashMessageContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"hashMessageContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string verifyMessageHash(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"verifyMessageHash"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getHashAlgorithm(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getHashAlgorithm"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string signHashedMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"signHashedMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
