#include "progressive/content_report_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildReport(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"buildReport"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string parseReportResponse(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"parseReportResponse"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string validateReportReason(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"validateReportReason"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string getReportCategories(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"getReportCategories"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string formatReportNotice(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"formatReportNotice"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

