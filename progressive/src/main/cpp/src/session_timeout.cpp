#include "progressive/session_timeout.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

SessionTimeout parseTimeoutConfig(const std::string& json) {
    SessionTimeout t;
    auto extractInt = [&](const std::string& key) -> int {
        auto p = json.find("\"" + key + "\":");
        if (p == std::string::npos) return 0;
        p += key.size() + 2;
        while (p < json.size() && json[p] == ' ') p++;
        try { return std::stoi(json.substr(p)); } catch(...) { return 0; }
    };
    t.inactivityMinutes = extractInt("inactivityMinutes");
    t.gracePeriodSeconds = extractInt("gracePeriodSeconds");
    if (t.gracePeriodSeconds <= 0) t.gracePeriodSeconds = 30;
    t.action = static_cast<TimeoutAction>(extractInt("action"));
    t.requireBiometric = json.find("\"requireBiometric\":true") != std::string::npos;
    return t;
}

static int64_t nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

bool shouldTimeout(const SessionTimeout& config, int64_t lastActivityMs, int64_t now) {
    if (config.inactivityMinutes <= 0) return false;
    if (now <= 0) now = nowMs();
    return (now - lastActivityMs) >= (config.inactivityMinutes * 60000LL);
}

int getRemainingSeconds(const SessionTimeout& config, int64_t lastActivityMs, int64_t now) {
    if (config.inactivityMinutes <= 0) return -1;
    if (now <= 0) now = nowMs();
    int64_t elapsed = now - lastActivityMs;
    int64_t timeoutMs = config.inactivityMinutes * 60000LL;
    if (elapsed >= timeoutMs) return 0;
    return (int)((timeoutMs - elapsed) / 1000);
}

std::string formatTimeoutWarning(int remainingSeconds) {
    if (remainingSeconds <= 0) return "Session will lock now";
    return "Session will lock in " + std::to_string(remainingSeconds) + " seconds";
}

std::string buildTimeoutConfig(const SessionTimeout& config) {
    std::ostringstream os;
    os << R"({"inactivityMinutes":)" << config.inactivityMinutes;
    os << R"(,"gracePeriodSeconds":)" << config.gracePeriodSeconds;
    os << R"(,"action":)" << static_cast<int>(config.action);
    os << R"(,"requireBiometric":)" << (config.requireBiometric ? "true" : "false");
    os << "}";
    return os.str();
}

bool isTimeoutEnabled(const SessionTimeout& config) {
    return config.inactivityMinutes > 0;
}

} // namespace progressive
