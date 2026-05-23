#include "progressive/avatar_fallback_utils.hpp"
#include <functional>
namespace progressive {
std::string computeInitials(const std::string& name, int max) {
    std::string r; for(char c:name){if(c!=' '&&c!='#'&&c!='!'&&(int)r.size()<max)r+=(char)toupper(c);} return r.empty()?"?":r;
}
std::string getAvatarColor(const std::string& id) {
    size_t h=std::hash<std::string>{}(id); const char* c[]={"#E53935","#1E88E5","#43A047","#FB8C00","#8E24AA","#00ACC1","#3949AB","#C0CA33"}; return c[h%8];
}
int getAvatarSizeDp(bool direct, bool small) { return small ? 24 : (direct ? 48 : 36); }
}
