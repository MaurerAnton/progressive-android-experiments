#include "progressive/device_rename_utils.hpp"
namespace progressive {
std::string buildDeviceRenameRequest(const std::string& did, const std::string& name){return R"({"device_id":")"+did+R"(","display_name":")"+name+R"("})";}
std::string parseDeviceRenameResponse(const std::string& json){auto p=json.find("\"display_name\":\"");if(p==std::string::npos)return"";p+=16;auto e=json.find('"',p);return e!=std::string::npos?json.substr(p,e-p):"";}
}
