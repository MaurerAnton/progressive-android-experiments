#include "progressive/user_report_utils.hpp"
#include <sstream>

std::string reportReasonToString(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"reportReasonToString"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildReportContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReportContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildReportServerNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReportServerNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatReportReason(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatReportReason"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
