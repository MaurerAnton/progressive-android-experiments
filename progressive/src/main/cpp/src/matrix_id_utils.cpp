#include "progressive/matrix_id_utils.hpp"
#include <sstream>

std::string validateUserId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"validateUserId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseUserId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseUserId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildUserId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildUserId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatMxid(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatMxid"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
