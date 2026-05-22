#include "progressive/permalink_utils.hpp"
#include <sstream>

namespace progressive {

std::string buildMatrixLink(const std::string& type, const std::string& id) {
    std::ostringstream os;
    os << "https://matrix.to/#/" << type << "/" << id;
    return os.str();
}

std::string buildRoomPermalink(const std::string& roomId, const std::string& via) {
    std::string link = buildMatrixLink("", roomId);
    if (!via.empty()) link += "?via=" + via;
    return link;
}

std::string buildUserPermalink(const std::string& userId) {
    return buildMatrixLink("", userId);
}

std::string buildEventPermalink(const std::string& roomId, const std::string& eventId,
                                  const std::string& via) {
    std::string link = "https://matrix.to/#/" + roomId + "/" + eventId;
    if (!via.empty()) link += "?via=" + via;
    return link;
}

bool parsePermalink(const std::string& url, std::string& type, std::string& id) {
    auto hashPos = url.find("#/");
    if (hashPos == std::string::npos) return false;
    std::string fragment = url.substr(hashPos + 2);
    auto slash = fragment.find('/');
    if (slash == std::string::npos) return false;
    type = fragment.substr(0, slash);
    id = fragment.substr(slash + 1);
    return true;
}

bool isPermalink(const std::string& url) {
    return url.find("matrix.to/#/") != std::string::npos;
}

std::string formatPermalink(const std::string& url, const std::string& label) {
    if (label.empty()) return url;
    return label + ": " + url;
}

} // namespace progressive
