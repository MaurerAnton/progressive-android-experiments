#include "progressive/room_alias_utils.hpp"
#include <sstream>

namespace progressive {

std::string buildAliasResolveRequest(const std::string& alias) {
    return R"({"room_alias":")" + alias + R"("})";
}

RoomAliasInfo parseAliasResolve(const std::string& json, const std::string& alias) {
    RoomAliasInfo info;
    info.alias = alias;
    auto ridPos = json.find("\"room_id\":\"");
    if (ridPos != std::string::npos) {
        ridPos += 11;
        auto ridEnd = json.find('"', ridPos);
        if (ridEnd != std::string::npos) info.roomId = json.substr(ridPos, ridEnd - ridPos);
    }
    info.isValid = !info.roomId.empty();
    return info;
}

bool isValidAlias(const std::string& alias) {
    return alias.size() > 3 && alias[0] == '#' && alias.find(':') != std::string::npos;
}

bool isAliasUrl(const std::string& url) {
    return url.find("#") == 0 || url.find("matrix.to/#/") != std::string::npos;
}

std::string extractServerFromAlias(const std::string& alias) {
    auto colon = alias.find(':');
    if (colon == std::string::npos) return "";
    return alias.substr(colon + 1);
}

std::string buildAliasCreateRequest(const std::string& alias) {
    return R"({"room_alias_name":")" + alias + R"("})";
}

std::vector<std::string> parseRoomAliases(const std::string& json) {
    std::vector<std::string> aliases;
    size_t pos = 0;
    while (pos < json.size()) {
        auto apos = json.find("\"#", pos);
        if (apos == std::string::npos) break;
        auto aend = json.find('"', apos + 1);
        if (aend == std::string::npos) break;
        aliases.push_back(json.substr(apos + 1, aend - apos - 1));
        pos = aend + 1;
    }
    return aliases;
}

std::string formatAlias(const std::string& alias) { return alias; }

std::string buildAliasJoinUrl(const std::string& alias) {
    return "https://matrix.to/#/" + alias;
}

} // namespace progressive
