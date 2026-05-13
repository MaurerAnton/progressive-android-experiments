#include "progressive/membership_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

Membership parseMembership(const std::string& membershipStr) {
    if (membershipStr == "join")     return Membership::Join;
    if (membershipStr == "invite")   return Membership::Invite;
    if (membershipStr == "leave")    return Membership::Leave;
    if (membershipStr == "ban")      return Membership::Ban;
    if (membershipStr == "knock")    return Membership::Knock;
    return Membership::Unknown;
}

MemberInfo parseMemberInfo(const std::string& stateContentJson, const std::string& userId) {
    MemberInfo info;
    info.userId = userId;

    auto membershipStr = parseJsonStringValue(stateContentJson, "membership");
    info.membership = parseMembership(membershipStr);

    info.displayName = parseJsonStringValue(stateContentJson, "displayname");
    info.avatarUrl   = parseJsonStringValue(stateContentJson, "avatar_url");
    info.reason      = parseJsonStringValue(stateContentJson, "reason");

    auto ts = parseJsonStringValue(stateContentJson, "origin_server_ts");
    if (!ts.empty()) info.timestampMs = std::stoll(ts);

    return info;
}

std::string formatMembership(Membership membership) {
    switch (membership) {
        case Membership::Join:  return "Joined";
        case Membership::Invite: return "Invited";
        case Membership::Leave: return "Left";
        case Membership::Ban:   return "Banned";
        case Membership::Knock: return "Knocked";
        default:                return "Unknown";
    }
}

bool isActiveMember(Membership membership) {
    return membership == Membership::Join ||
           membership == Membership::Invite ||
           membership == Membership::Knock;
}

bool canReadMessages(Membership membership) {
    return membership == Membership::Join;
}

MembershipChange detectMembershipChange(const MemberInfo& oldInfo, const MemberInfo& newInfo) {
    MembershipChange change;
    change.userId = oldInfo.userId;
    change.displayName = newInfo.displayName.empty() ? oldInfo.displayName : newInfo.displayName;
    change.oldMembership = oldInfo.membership;
    change.newMembership = newInfo.membership;
    change.timestampMs = newInfo.timestampMs;
    return change;
}

std::string formatMembershipChange(const MembershipChange& change) {
    std::ostringstream out;
    out << change.displayName;
    if (change.oldMembership == Membership::Unknown) {
        out << " " << formatMembership(change.newMembership);
    } else {
        out << " changed from " << formatMembership(change.oldMembership)
            << " to " << formatMembership(change.newMembership);
    }
    return out.str();
}

MemberListInfo parseMemberList(const std::string& roomId, const std::string& apiResponseJson, bool isTruncated) {
    MemberListInfo list;
    list.roomId = roomId;
    list.isTruncated = isTruncated;

    // Parse each chunk/event
    size_t pos = 0;
    while (true) {
        pos = apiResponseJson.find("\"user_id\"", pos);
        if (pos == std::string::npos) {
            pos = apiResponseJson.find("\"sender\"", pos);
            if (pos == std::string::npos) break;
        }

        auto objStart = apiResponseJson.rfind('{', pos);
        if (objStart == std::string::npos) break;

        int depth = 0;
        auto objEnd = objStart;
        while (objEnd < apiResponseJson.size()) {
            if (apiResponseJson[objEnd] == '{') ++depth;
            else if (apiResponseJson[objEnd] == '}') --depth;
            if (depth == 0) break;
            ++objEnd;
        }
        if (objEnd >= apiResponseJson.size()) break;

        std::string obj = apiResponseJson.substr(objStart, objEnd - objStart + 1);

        MemberInfo info;
        info.userId      = parseJsonStringValue(obj, "user_id");
        if (info.userId.empty()) info.userId = parseJsonStringValue(obj, "sender");
        info.displayName = parseJsonStringValue(obj, "display_name");
        if (info.displayName.empty()) {
            auto content = parseJsonStringValue(obj, "content");
            if (!content.empty()) {
                info.displayName = parseJsonStringValue("{" + content + "}", "displayname");
                info.avatarUrl   = parseJsonStringValue("{" + content + "}", "avatar_url");
                auto ms = parseJsonStringValue("{" + content + "}", "membership");
                info.membership = parseMembership(ms);
            }
        }
        info.avatarUrl = parseJsonStringValue(obj, "avatar_url");

        if (!info.userId.empty()) list.members.push_back(info);
        pos = objEnd + 1;
    }

    list.totalMembers = static_cast<int>(list.members.size());
    for (const auto& m : list.members) {
        switch (m.membership) {
            case Membership::Join:   list.joinedMembers++; break;
            case Membership::Invite: list.invitedMembers++; break;
            case Membership::Ban:    list.bannedMembers++; break;
            default: break;
        }
    }

    return list;
}

std::vector<MemberInfo> filterByMembership(const std::vector<MemberInfo>& members, Membership type) {
    std::vector<MemberInfo> result;
    for (const auto& m : members) {
        if (m.membership == type) result.push_back(m);
    }
    return result;
}

std::vector<MemberInfo> searchMembers(const std::vector<MemberInfo>& members, const std::string& query) {
    if (query.empty()) return members;
    auto lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    std::vector<MemberInfo> result;
    for (const auto& m : members) {
        auto lowerName = m.displayName;
        auto lowerId = m.userId;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        std::transform(lowerId.begin(), lowerId.end(), lowerId.begin(), ::tolower);
        if (lowerName.find(lowerQuery) != std::string::npos ||
            lowerId.find(lowerQuery) != std::string::npos) {
            result.push_back(m);
        }
    }
    return result;
}

void sortMembers(std::vector<MemberInfo>& members, const std::string& sortBy) {
    if (sortBy == "power") {
        std::sort(members.begin(), members.end(), [](const auto& a, const auto& b) {
            return a.powerLevel > b.powerLevel;
        });
    } else if (sortBy == "date") {
        std::sort(members.begin(), members.end(), [](const auto& a, const auto& b) {
            return a.timestampMs > b.timestampMs;
        });
    } else { // name
        std::sort(members.begin(), members.end(), [](const auto& a, const auto& b) {
            auto na = a.displayName;
            auto nb = b.displayName;
            std::transform(na.begin(), na.end(), na.begin(), ::tolower);
            std::transform(nb.begin(), nb.end(), nb.begin(), ::tolower);
            return na < nb;
        });
    }
}

std::string memberListToJson(const MemberListInfo& list) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"roomId": ")" << esc(list.roomId) << R"(")";
    json << R"(,"totalMembers": )" << list.totalMembers << ",";
    json << R"(,"joined": )" << list.joinedMembers << ",";
    json << R"(,"invited": )" << list.invitedMembers;
    json << "}";
    return json.str();
}

} // namespace progressive
