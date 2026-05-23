#include "progressive/room_knock_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildKnockRequest(const std::string& rid, const std::vector<std::string>& via, const std::string& reason){std::ostringstream os;os<<R"({"room_id":")"<<rid<<R"(")";if(!via.empty()){os<<R"(,"via":[)";for(size_t i=0;i<via.size();i++){if(i>0)os<<",";os<<R"(")"<<via[i]<<R"(")";}os<<"]";}if(!reason.empty())os<<R"(,"reason":")"<<reason<<R"(")";os<<"}";return os.str();}
std::string formatKnockResponse(const std::string& json){return json.find("\"room_id\"")!=std::string::npos?"Knocked successfully":"Knock failed";}
}
