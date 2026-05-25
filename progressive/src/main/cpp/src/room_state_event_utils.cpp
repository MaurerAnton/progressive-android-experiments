#include "progressive/room_state_event_utils.hpp"
#include <sstream>

std::string buildRoomNameEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRoomNameEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildRoomTopicEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRoomTopicEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildGuestAccessEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildGuestAccessEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildHistoryVisibilityEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildHistoryVisibilityEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
