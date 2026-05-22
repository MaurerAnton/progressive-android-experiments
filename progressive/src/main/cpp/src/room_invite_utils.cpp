#include "progressive/room_invite_utils.hpp"
#include <sstream>

namespace progressive {

std::string buildInviteRequest(const std::string& userId, const std::string& reason) {
    std::ostringstream os;
    os << R"({"user_id":")" << userId << R"(")";
    if (!reason.empty()) os << R"(,"reason":")" << reason << R"(")";
    os << "}";
    return os.str();
}

InviteResult parseInviteResponse(const std::string& json, const std::string& roomId) {
    InviteResult r;
    r.roomId = roomId;
    r.success = json.empty() || json == "{}" || json.find("\"room_id\"") != std::string::npos;
    if (!r.success) {
        auto errPos = json.find("\"error\":\"");
        if (errPos != std::string::npos) {
            errPos += 9;
            auto errEnd = json.find('"', errPos);
            if (errEnd != std::string::npos) r.error = json.substr(errPos, errEnd - errPos);
        }
    }
    return r;
}

std::string build3pidInviteRequest(const std::string& address, const std::string& medium,
                                      const std::string& idServer, const std::string& idAccessToken) {
    std::ostringstream os;
    os << R"({"id_server":")" << idServer << R"(",)";
    os << R"("id_access_token":")" << idAccessToken << R"(",)";
    os << R"("medium":")" << medium << R"(",)";
    os << R"("address":")" << address << R"(")";
    os << "}";
    return os.str();
}

bool isValidInviteTarget(const std::string& input) {
    if (input.empty()) return false;
    if (input[0] == '@' && input.find(':') != std::string::npos) return true; // MXID
    if (input.find('@') != std::string::npos && input.find('.') != std::string::npos) return true; // email
    return false;
}

std::string formatInviteNotice(const std::string& inviterName, const std::string& roomName) {
    std::ostringstream os;
    os << inviterName << " invited you to " << (roomName.empty() ? "a room" : roomName);
    return os.str();
}

} // namespace progressive
