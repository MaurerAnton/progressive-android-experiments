#include "progressive/member_list.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    if (e == std::string::npos) return "";
    return json.substr(p, e - p);
}

static int extractInt(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":");
    if (p == std::string::npos) return 0;
    p += key.size() + 2;
    while (p < json.size() && json[p] == ' ') p++;
    try { return std::stoi(json.substr(p)); } catch (...) { return 0; }
}

MemberInfo parseMemberEvent(const std::string& userId, const std::string& eventJson,
                              const std::string& prevContentJson) {
    MemberInfo m;
    m.userId = userId;
    m.membership = extractField(eventJson, "membership");
    m.displayName = extractField(eventJson, "displayname");
    m.avatarUrl = extractField(eventJson, "avatar_url");
    m.reason = extractField(eventJson, "reason");
    return m;
}

std::string buildMemberContent(const MemberInfo& member, const std::string& reason) {
    std::ostringstream os;
    os << "{";
    os << R"("membership":")" << member.membership << R"(")";
    if (!member.displayName.empty()) os << R"(,"displayname":")" << member.displayName << R"(")";
    if (!member.avatarUrl.empty()) os << R"(,"avatar_url":")" << member.avatarUrl << R"(")";
    if (!reason.empty()) os << R"(,"reason":")" << reason << R"(")";
    os << "}";
    return os.str();
}

MemberList parseMemberList(const std::string& json) {
    MemberList list;
    size_t pos = 0;
    while (true) {
        pos = json.find("\"user_id\":\"", pos);
        if (pos == std::string::npos) break;
        pos += 11;
        auto end = json.find('"', pos);
        if (end == std::string::npos) break;
        std::string userId = json.substr(pos, end - pos);
        
        MemberInfo m;
        m.userId = userId;
        m.displayName = extractField(json, "display_name");
        m.avatarUrl = extractField(json, "avatar_url");
        m.membership = extractField(json, "membership");
        
        list.members.push_back(m);
        if (m.membership == "join") list.joinedCount++;
        if (m.membership == "invite") list.invitedCount++;
        list.totalCount++;
        pos = end + 1;
    }
    return list;
}

std::vector<MemberInfo> filterMembers(const std::vector<MemberInfo>& members,
                                       const std::string& membership,
                                       const std::string& nameQuery,
                                       int minPowerLevel) {
    std::vector<MemberInfo> result;
    for (const auto& m : members) {
        if (!membership.empty() && m.membership != membership) continue;
        if (!nameQuery.empty()) {
            std::string lower;
            std::transform(m.displayName.begin(), m.displayName.end(), std::back_inserter(lower), ::tolower);
            std::string q;
            std::transform(nameQuery.begin(), nameQuery.end(), std::back_inserter(q), ::tolower);
            if (lower.find(q) == std::string::npos) continue;
        }
        if (minPowerLevel >= 0 && m.powerLevel < minPowerLevel) continue;
        result.push_back(m);
    }
    return result;
}

std::vector<MemberInfo> sortMembers(const std::vector<MemberInfo>& members, MemberSort mode) {
    std::vector<MemberInfo> sorted = members;
    switch (mode) {
        case MemberSort::NAME:
            std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
                return a.displayName < b.displayName;
            });
            break;
        case MemberSort::POWER_LEVEL:
            std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
                return a.powerLevel > b.powerLevel;
            });
            break;
        case MemberSort::JOIN_DATE:
            std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
                return a.joinedAtMs > b.joinedAtMs;
            });
            break;
        case MemberSort::LAST_ACTIVE:
            std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
                return a.lastActiveMs > b.lastActiveMs;
            });
            break;
    }
    return sorted;
}

std::string formatMemberDisplayName(const MemberInfo& member) {
    if (!member.displayName.empty()) return member.displayName;
    auto colon = member.userId.find(':');
    return member.userId.substr(1, colon - 1);
}

std::string memberToJson(const MemberInfo& m) {
    std::ostringstream os;
    os << "{";
    os << R"("userId":")" << m.userId << R"(")";
    os << R"(,"displayName":")" << m.displayName << R"(")";
    os << R"(,"membership":")" << m.membership << R"(")";
    os << R"(,"powerLevel":)" << m.powerLevel;
    os << "}";
    return os.str();
}

std::string memberListToJson(const MemberList& list) {
    std::ostringstream os;
    os << R"({"members":[)";
    for (size_t i = 0; i < list.members.size(); i++) {
        if (i > 0) os << ",";
        os << memberToJson(list.members[i]);
    }
    os << R"(],"total":)" << list.totalCount;
    os << R"(,"joined":)" << list.joinedCount;
    os << R"(,"invited":)" << list.invitedCount;
    os << "}";
    return os.str();
}

} // namespace progressive
