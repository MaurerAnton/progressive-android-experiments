#include "progressive/room_unread_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildUnreadMarker(const std::string& eid, int count){std::ostringstream os;os<<R"({"m.fully_read":")"<<eid<<R"(","notification_count":)"<<count<<"}";return os.str();}
bool hasUnreadMessages(const std::string&, int c){return c>0;}
std::string formatUnreadBadge(int c, int h){if(c<=0)return"";if(c>=1000)return"999+";return std::to_string(c);}
}
