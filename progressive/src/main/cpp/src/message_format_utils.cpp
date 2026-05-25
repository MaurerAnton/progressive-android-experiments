#include "progressive/message_format_utils.hpp"
#include <sstream>

std::string formatHtmlToPlain(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatHtmlToPlain"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatPlainToHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatPlainToHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string sanitizeHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sanitizeHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
