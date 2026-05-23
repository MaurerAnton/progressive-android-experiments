#include "progressive/server_acronym_utils.hpp"
#include <algorithm>
namespace progressive {
std::string getServerAcronym(const std::string& name) {
    std::string r;
    for (char c : name) { if (c == '.') r += (char)toupper(name[r.size() > 0 ? r.size() : 0]); }
    if (r.empty() && !name.empty()) r = (char)toupper(name[0]);
    return r;
}
std::string formatServerName(const std::string& name) {
    if (name == "matrix.org") return "Matrix.org";
    return name;
}
bool isDefaultServer(const std::string& name) { return name == "matrix.org"; }
}
