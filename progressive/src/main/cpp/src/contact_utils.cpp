#include "progressive/contact_utils.hpp"
#include <algorithm>
namespace progressive {
std::string normalizePhoneNumber(const std::string& p){std::string r;for(char c:p)if(isdigit(c)||c=='+')r+=c;return r;}
std::string formatPhoneForDisplay(const std::string& p){if(p.size()<10)return p;return p.substr(0,3)+"-"+p.substr(3,3)+"-"+p.substr(6);}
bool isValidEmailAddress(const std::string& e){auto a=e.find('@');return a!=std::string::npos&&a>0&&a<e.size()-1&&e.find('.',a)!=std::string::npos;}
}
