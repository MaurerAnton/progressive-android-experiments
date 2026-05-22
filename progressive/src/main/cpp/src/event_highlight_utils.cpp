#include "progressive/event_highlight_utils.hpp"
#include <algorithm>

namespace progressive {
static std::string toLower(const std::string& s) { std::string r; std::transform(s.begin(),s.end(),std::back_inserter(r),::tolower); return r; }
bool containsDisplayName(const std::string& body, const std::string& name) {
    return toLower(body).find(toLower(name)) != std::string::npos;
}
bool matchesHighlightRules(const std::string& body, const std::vector<std::string>& keywords) {
    std::string lower = toLower(body);
    for (const auto& kw : keywords) if (lower.find(toLower(kw)) != std::string::npos) return true;
    return false;
}
int parseHighlightCount(const std::string& json) {
    auto p = json.find("\"highlight_count\":"); if (p == std::string::npos) return 0;
    p += 18; try { return std::stoi(json.substr(p)); } catch(...) { return 0; }
}
} // namespace progressive
