#include "progressive/user_badge_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string getUserBadge(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"getUserBadge"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string getPowerBadge(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"getPowerBadge"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string isAdmin(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"isAdmin"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string isModerator(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"isModerator"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string formatBadgeHtml(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"formatBadgeHtml"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

