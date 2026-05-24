#include "progressive/user_status_message_utils.hpp"
#include <sstream>
#include <chrono>
namespace progressive {
std::string buildStatusMessage(const std::string& s, const std::string& e) {
    std::ostringstream os; os << R"({"status":")" << s << R"(")";
    if (!e.empty()) os << R"(,"emoji":")" << e << R"(")"; os << "}"; return os.str();
}
std::string parseStatusMessage(const std::string& json) {
    auto p = json.find("\"status\":\""); if (p == std::string::npos) return "";
    p += 10; auto e = json.find('"', p); return e != std::string::npos ? json.substr(p, e - p) : "";
}
bool isStatusExpired(int64_t setMs, int64_t maxAge) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (now - setMs) > maxAge;
}
}
