#include "progressive/content_warning_utils.hpp"
namespace progressive {
std::string buildContentWarning(const std::string& r){return R"({"m.content.warning":")"+r+R"("})";}
std::string parseContentWarning(const std::string& json){auto p=json.find("\"m.content.warning\":\"");if(p==std::string::npos)return"";p+=21;auto e=json.find('"',p);return e!=std::string::npos?json.substr(p,e-p):"";}
bool hasContentWarning(const std::string& json){return json.find("\"m.content.warning\"")!=std::string::npos;}
}
