#include "progressive/event_forward_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildForwardRequest(const std::vector<std::string>& ids){std::ostringstream os;os<<R"({"room_ids":[)";for(size_t i=0;i<ids.size();i++){if(i>0)os<<",";os<<R"(")"<<ids[i]<<R"(")";}os<<"]}";return os.str();}
std::string buildForwardContent(const std::string& eid, const std::string& rid){return R"({"event_id":")"+eid+R"(","room_id":")"+rid+R"("})";}
}
