#include "progressive/push_gateway_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildPusherRequest(const std::string& pk, const std::string& aid, const std::string& an, const std::string& dn, const std::string& pt, const std::string& l, const std::string& u, const std::string& f) {
    std::ostringstream os; os<<R"({"pushkey":")"<<pk<<R"(","app_id":")"<<aid<<R"(","app_name":")"<<an<<R"(")";
    os<<R"(,"device_name":")"<<dn<<R"(","profile_tag":")"<<pt<<R"(","lang":")"<<l<<R"(")";
    os<<R"(,"kind":"http","app_display_name":")"<<an<<R"(","url":")"<<u<<R"(")";
    os<<R"(,"format":")"<<f<<R"(")"; os<<"}"; return os.str();
}
std::string parsePusherResponse(const std::string& json){return json;}
}
