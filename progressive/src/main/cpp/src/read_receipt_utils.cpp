#include "progressive/read_receipt_utils.hpp"
#include <sstream>

std::string parseReadReceipt(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseReadReceipt"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildReceiptEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReceiptEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getReadPosition(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getReadPosition"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getUnreadCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getUnreadCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
