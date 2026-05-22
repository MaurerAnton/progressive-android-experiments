#include "progressive/message_reply_utils.hpp"
#include <sstream>

namespace progressive {

std::string buildReplyFormattedBody(const std::string& rBody, const std::string& rFmt,
                                      const std::string& rSender, const std::string& rUid,
                                      const std::string& nBody, const std::string& nFmt) {
    std::ostringstream os;
    os << "<mx-reply><blockquote>";
    os << "<a href=\"https://matrix.to/#/" << rUid << "\">" << rSender << "</a><br/>";
    os << (rFmt.empty() ? rBody : rFmt);
    os << "</blockquote></mx-reply>";
    os << (nFmt.empty() ? nBody : nFmt);
    return os.str();
}
std::string buildReplyPlainBody(const std::string& rBody, const std::string& nBody) {
    std::istringstream iss(rBody); std::string line; std::ostringstream os;
    while (std::getline(iss, line)) os << "> " << line << "\n";
    os << "\n" << nBody; return os.str();
}
std::string extractRepliedEventId(const std::string& json) {
    auto p = json.find("\"m.in_reply_to\""); if (p == std::string::npos) return "";
    p = json.find("\"event_id\":\"", p); if (p == std::string::npos) return "";
    p += 12; auto e = json.find('"', p); return e != std::string::npos ? json.substr(p, e - p) : "";
}
bool isReplyEvent(const std::string& json) { return json.find("\"m.in_reply_to\"") != std::string::npos; }

} // namespace progressive
