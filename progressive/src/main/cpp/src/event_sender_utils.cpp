#include "progressive/event_sender_utils.hpp"
#include <sstream>
namespace progressive {
std::string getSenderDisplayName(const std::string& json, const std::string& fb) {
    auto p = json.find("\"sender\":\""); if (p == std::string::npos) return fb;
    p += 10; auto e = json.find('"', p); return e != std::string::npos ? json.substr(p, e - p) : fb;
}
bool isOwnEvent(const std::string& sid, const std::string& myId) { return sid == myId; }
std::string formatSenderAvatarUrl(const std::string& mxc, const std::string& hs, int size) {
    if (mxc.empty() || mxc.find("mxc://") != 0) return mxc;
    auto slash = mxc.find('/', 6); std::string server = mxc.substr(6, slash-6);
    std::string mediaId = mxc.substr(slash+1);
    std::string base = hs; while (!base.empty() && base.back()=='/') base.pop_back();
    std::ostringstream os;
    os << base << "/_matrix/media/v3/thumbnail/" << server << "/" << mediaId
       << "?width=" << size << "&height=" << size << "&method=crop";
    return os.str();
}
}
