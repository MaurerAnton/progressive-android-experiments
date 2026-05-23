#include "progressive/direct_message_utils.hpp"
namespace progressive {
std::string buildDirectMessageFlag(const std::string& uid, const std::string& rid){return R"({")"+uid+R"(":[")"+rid+R"("]})";}
bool isDirectMessage(const std::string& json, const std::string& rid){return json.find(rid)!=std::string::npos;}
}
