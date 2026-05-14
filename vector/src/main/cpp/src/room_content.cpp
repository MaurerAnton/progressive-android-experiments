#include "progressive/room_content.hpp"

namespace progressive {

// ==== Membership ====

const char* membershipToString(Membership m) {
    switch (m) {
        case Membership::NONE: return "none";
        case Membership::INVITE: return "invite";
        case Membership::JOIN: return "join";
        case Membership::KNOCK: return "knock";
        case Membership::LEAVE: return "leave";
        case Membership::BAN: return "ban";
    }
    return "none";
}

Membership membershipFromString(const std::string& s) {
    if (s == "invite") return Membership::INVITE;
    if (s == "join") return Membership::JOIN;
    if (s == "knock") return Membership::KNOCK;
    if (s == "leave") return Membership::LEAVE;
    if (s == "ban") return Membership::BAN;
    return Membership::NONE;
}

// ==== Room Join Rules ====

const char* roomJoinRulesToString(RoomJoinRules r) {
    switch (r) {
        case RoomJoinRules::PUBLIC: return "public";
        case RoomJoinRules::INVITE: return "invite";
        case RoomJoinRules::KNOCK: return "knock";
        case RoomJoinRules::PRIVATE: return "private";
        case RoomJoinRules::RESTRICTED: return "restricted";
    }
    return "invite";
}

RoomJoinRules roomJoinRulesFromString(const std::string& s) {
    if (s == "public") return RoomJoinRules::PUBLIC;
    if (s == "invite") return RoomJoinRules::INVITE;
    if (s == "knock") return RoomJoinRules::KNOCK;
    if (s == "private") return RoomJoinRules::PRIVATE;
    if (s == "restricted") return RoomJoinRules::RESTRICTED;
    return RoomJoinRules::INVITE;
}

// ==== Room History Visibility ====

const char* roomHistoryVisibilityToString(RoomHistoryVisibility v) {
    switch (v) {
        case RoomHistoryVisibility::WORLD_READABLE: return "world_readable";
        case RoomHistoryVisibility::SHARED: return "shared";
        case RoomHistoryVisibility::INVITED: return "invited";
        case RoomHistoryVisibility::JOINED: return "joined";
    }
    return "shared";
}

RoomHistoryVisibility roomHistoryVisibilityFromString(const std::string& s) {
    if (s == "world_readable") return RoomHistoryVisibility::WORLD_READABLE;
    if (s == "shared") return RoomHistoryVisibility::SHARED;
    if (s == "invited") return RoomHistoryVisibility::INVITED;
    if (s == "joined") return RoomHistoryVisibility::JOINED;
    return RoomHistoryVisibility::SHARED;
}

// ==== Guest Access ====

const char* guestAccessToString(GuestAccess g) {
    switch (g) {
        case GuestAccess::CAN_JOIN: return "can_join";
        case GuestAccess::FORBIDDEN: return "forbidden";
    }
    return "forbidden";
}

GuestAccess guestAccessFromString(const std::string& s) {
    if (s == "can_join") return GuestAccess::CAN_JOIN;
    return GuestAccess::FORBIDDEN;
}

// ==== Versioning State ====

const char* roomVersioningStateToString(RoomVersioningState s) {
    switch (s) {
        case RoomVersioningState::NONE: return "NONE";
        case RoomVersioningState::UPGRADED_ROOM_NOT_JOINED: return "UPGRADED_ROOM_NOT_JOINED";
        case RoomVersioningState::UPGRADED_ROOM_JOINED: return "UPGRADED_ROOM_JOINED";
    }
    return "NONE";
}

RoomVersioningState roomVersioningStateFromString(const std::string& s) {
    if (s == "UPGRADED_ROOM_NOT_JOINED") return RoomVersioningState::UPGRADED_ROOM_NOT_JOINED;
    if (s == "UPGRADED_ROOM_JOINED") return RoomVersioningState::UPGRADED_ROOM_JOINED;
    return RoomVersioningState::NONE;
}

// ==== JSON Helpers ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int extractJsonInt(const std::string& json, const std::string& key, int defaultVal = 0) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return defaultVal;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return defaultVal;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return defaultVal;
    int val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

// ==== Parse RoomMemberContent ====
//
// Original Kotlin (RoomMemberContent.kt:25-41)

RoomMemberContent parseRoomMemberContent(const std::string& json) {
    RoomMemberContent c;
    c.membership = membershipFromString(extractJsonString(json, "membership"));
    c.reason = extractJsonString(json, "reason");
    c.displayName = extractJsonString(json, "displayname");
    c.avatarUrl = extractJsonString(json, "avatar_url");
    c.isDirect = extractJsonBool(json, "is_direct");

    auto inviteJson = extractJsonObject(json, "third_party_invite");
    if (!inviteJson.empty()) {
        c.thirdPartyInvite.displayName = extractJsonString(inviteJson, "display_name");
        c.thirdPartyInvite.signedToken = extractJsonString(inviteJson, "signed");
    }

    c.unsignedData = extractJsonObject(json, "unsigned");
    return c;
}

// ==== Parse PowerLevelsContent ====
//
// Original Kotlin (PowerLevelsContent.kt:27-68)

RoomPowerLevelsContent parsePowerLevelsContent(const std::string& json) {
    RoomPowerLevelsContent c;
    c.ban = extractJsonInt(json, "ban", 50);
    c.kick = extractJsonInt(json, "kick", 50);
    c.invite = extractJsonInt(json, "invite", 0);
    c.redact = extractJsonInt(json, "redact", 50);
    c.eventsDefault = extractJsonInt(json, "events_default", 0);
    c.usersDefault = extractJsonInt(json, "users_default", 0);
    c.stateDefault = extractJsonInt(json, "state_default", 50);

    // Parse "events" map
    auto eventsJson = extractJsonObject(json, "events");
    if (!eventsJson.empty()) {
        size_t pos = 1;
        while (pos < eventsJson.size()) {
            while (pos < eventsJson.size() && (eventsJson[pos] == ' ' || eventsJson[pos] == ',')) pos++;
            if (pos >= eventsJson.size() || eventsJson[pos] == '}') break;
            if (eventsJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < eventsJson.size() && eventsJson[keyEnd] != '"') keyEnd++;
                std::string key = eventsJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < eventsJson.size() && eventsJson[pos] != ':') pos++;
                pos++;
                while (pos < eventsJson.size() && (eventsJson[pos] == ' ' || eventsJson[pos] == '\t')) pos++;
                int val = 0;
                while (pos < eventsJson.size() && eventsJson[pos] >= '0' && eventsJson[pos] <= '9') {
                    val = val * 10 + (eventsJson[pos] - '0');
                    pos++;
                }
                c.events[key] = val;
            }
        }
    }

    // Parse "users" map
    auto usersJson = extractJsonObject(json, "users");
    if (!usersJson.empty()) {
        size_t pos = 1;
        while (pos < usersJson.size()) {
            while (pos < usersJson.size() && (usersJson[pos] == ' ' || usersJson[pos] == ',')) pos++;
            if (pos >= usersJson.size() || usersJson[pos] == '}') break;
            if (usersJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < usersJson.size() && usersJson[keyEnd] != '"') keyEnd++;
                std::string key = usersJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < usersJson.size() && usersJson[pos] != ':') pos++;
                pos++;
                while (pos < usersJson.size() && (usersJson[pos] == ' ' || usersJson[pos] == '\t')) pos++;
                int val = 0;
                while (pos < usersJson.size() && usersJson[pos] >= '0' && usersJson[pos] <= '9') {
                    val = val * 10 + (usersJson[pos] - '0');
                    pos++;
                }
                c.users[key] = val;
            }
        }
    }

    // Parse notifications.room level
    auto notifJson = extractJsonObject(json, "notifications");
    if (!notifJson.empty()) {
        c.notificationRoomLevel = extractJsonInt(notifJson, "room", 50);
    }

    return c;
}

// ==== Simple Parse Functions ====

RoomNameContent parseRoomNameContent(const std::string& json) {
    // Original Kotlin (RoomNameContent.kt:24-26): @Json(name="name") name
    return {extractJsonString(json, "name")};
}

RoomTopicContent parseRoomTopicContent(const std::string& json) {
    // Original Kotlin (RoomTopicContent.kt:22-24): @Json(name="topic") topic
    return {extractJsonString(json, "topic")};
}

RoomAvatarContent parseRoomAvatarContent(const std::string& json) {
    // Original Kotlin (RoomAvatarContent.kt:22-24): @Json(name="url") avatarUrl
    return {extractJsonString(json, "url")};
}

RoomCanonicalAliasContent parseRoomCanonicalAliasContent(const std::string& json) {
    // Original Kotlin (RoomCanonicalAliasContent.kt:25-37)
    RoomCanonicalAliasContent c;
    c.canonicalAlias = extractJsonString(json, "alias");
    // Parse alt_aliases array
    auto pos = json.find("\"alt_aliases\"");
    if (pos != std::string::npos) {
        pos = json.find('[', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size()) {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',' || json[pos] == '\n')) pos++;
                if (pos >= json.size() || json[pos] == ']') break;
                if (json[pos] == '"') {
                    pos++;
                    size_t end = pos;
                    while (end < json.size() && json[end] != '"') end++;
                    c.alternativeAliases.push_back(json.substr(pos, end - pos));
                    pos = end + 1;
                }
            }
        }
    }
    return c;
}

RoomJoinRulesContent parseRoomJoinRulesContent(const std::string& json) {
    // Original Kotlin (RoomJoinRulesContent.kt:26-43)
    RoomJoinRulesContent c;
    c.joinRules = roomJoinRulesFromString(extractJsonString(json, "join_rule"));
    return c;
}

RoomHistoryVisibilityContent parseRoomHistoryVisibilityContent(const std::string& json) {
    // Original Kotlin (RoomHistoryVisibilityContent.kt:24-33)
    RoomHistoryVisibilityContent c;
    c.historyVisibility = roomHistoryVisibilityFromString(extractJsonString(json, "history_visibility"));
    return c;
}

RoomGuestAccessContent parseRoomGuestAccessContent(const std::string& json) {
    // Original Kotlin (RoomGuestAccessContent.kt:30-37)
    RoomGuestAccessContent c;
    c.guestAccess = guestAccessFromString(extractJsonString(json, "guest_access"));
    return c;
}

} // namespace progressive
