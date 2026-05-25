#include "progressive/widget_url_check.hpp"
#include <sstream>

std::string validateWidgetUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"validateWidgetUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isAuthorizedDomain(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isAuthorizedDomain"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseWidgetParams(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseWidgetParams"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string sanitizeWidgetHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sanitizeWidgetHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
