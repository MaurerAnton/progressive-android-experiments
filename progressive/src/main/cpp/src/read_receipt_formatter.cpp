#include "progressive/read_receipt_formatter.hpp"
#include <sstream>

std::string formatReadReceipts(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatReadReceipts"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getReadCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getReadCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isFullyRead(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isFullyRead"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getLastReadEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getLastReadEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
