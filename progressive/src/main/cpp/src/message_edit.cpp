#include "progressive/message_edit.hpp"
#include <sstream>

std::string canEdit(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canEdit"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseEditInfo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseEditInfo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getEditDiff(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getEditDiff"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatEditNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatEditNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
