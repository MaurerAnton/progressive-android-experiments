#include "progressive/room_history_utils.hpp"
namespace progressive {
bool canUserReadHistory(const std::string& vis, const std::string& mem, bool wasInvited) {
    if (mem=="join") return true; if (vis=="world_readable") return true;
    if (vis=="shared" && wasInvited) return true; if (vis=="invited" && mem=="invite") return true; return false;
}
std::string getDefaultHistoryVisibility(){return "shared";}
std::string formatHistoryVisibility(const std::string& v){
    if(v=="world_readable")return"Anyone";if(v=="shared")return"Members (since invited)";
    if(v=="invited")return"Members (since joined)";if(v=="joined")return"Members only";return v;
}
}
