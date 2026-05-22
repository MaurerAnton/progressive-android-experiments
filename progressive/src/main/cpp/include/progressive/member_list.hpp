#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

struct MemberInfo {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    std::string membership;     // "join", "invite", "leave", "ban", "knock"
    std::string reason;         // reason for membership change
    int powerLevel = 0;
    int64_t joinedAtMs = 0;
    int64_t lastActiveMs = 0;
    bool isOnline = false;
    bool isTyping = false;
};

struct MemberList {
    std::vector<MemberInfo> members;
    int totalCount = 0;
    int joinedCount = 0;
    int invitedCount = 0;
    bool isTruncated = false;
    std::string nextBatch;      // pagination token
};

// Parse m.room.member event content
MemberInfo parseMemberEvent(const std::string& userId, const std::string& eventJson,
                             const std::string& prevContentJson = "");
std::string buildMemberContent(const MemberInfo& member, const std::string& reason = "");

// Parse /members API response
MemberList parseMemberList(const std::string& json);

// Filter members by criteria
std::vector<MemberInfo> filterMembers(const std::vector<MemberInfo>& members,
                                       const std::string& membership = "",
                                       const std::string& nameQuery = "",
                                       int minPowerLevel = -1);

// Sort members
enum class MemberSort { NAME, POWER_LEVEL, JOIN_DATE, LAST_ACTIVE };
std::vector<MemberInfo> sortMembers(const std::vector<MemberInfo>& members, MemberSort mode);

// Format for display
std::string formatMemberDisplayName(const MemberInfo& member);
std::string memberListToJson(const MemberList& list);
std::string memberToJson(const MemberInfo& member);

// Check if user can perform action based on power level
bool canInvite(int pl) { return pl >= 0; }
bool canKick(int pl) { return pl >= 50; }
bool canBan(int pl) { return pl >= 50; }
bool canSetPowerLevels(int pl) { return pl >= 100; }
bool canChangeJoinRules(int pl) { return pl >= 100; }

} // namespace progressive
