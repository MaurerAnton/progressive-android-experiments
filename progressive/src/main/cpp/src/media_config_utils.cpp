#include "progressive/media_config_utils.hpp"
namespace progressive {
int64_t getMaxUploadSize(const std::string& json){auto p=json.find("\"m.upload.size\":");if(p==std::string::npos)return 50LL*1024*1024;p+=16;try{return std::stoll(json.substr(p));}catch(...){return 50LL*1024*1024;}}
std::string getMediaUploadUrl(const std::string& hs){std::string b=hs;while(!b.empty()&&b.back()=='/')b.pop_back();return b+"/_matrix/media/v3/upload";}
bool isMimeTypeSupported(const std::string& m, const std::string&){return !m.empty();}
}
