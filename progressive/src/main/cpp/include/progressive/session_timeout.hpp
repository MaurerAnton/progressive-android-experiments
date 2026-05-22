#pragma once
#include <string>
#include <cstdint>

namespace progressive {

enum class TimeoutAction {
    NONE = 0,
    LOCK_APP = 1,
    LOGOUT = 2
};

struct SessionTimeout {
    int inactivityMinutes = 0;     // 0 = disabled
    int gracePeriodSeconds = 30;   // warning before timeout
    TimeoutAction action = TimeoutAction::LOCK_APP;
    bool requireBiometric = false; // require biometric to unlock
};

// Parse timeout config from preferences JSON
SessionTimeout parseTimeoutConfig(const std::string& json);

// Check if session should timeout based on last activity
bool shouldTimeout(const SessionTimeout& config, int64_t lastActivityMs, int64_t nowMs = 0);

// Get remaining time before timeout in seconds
int getRemainingSeconds(const SessionTimeout& config, int64_t lastActivityMs, int64_t nowMs = 0);

// Format timeout warning message
std::string formatTimeoutWarning(int remainingSeconds);

// Build timeout config JSON
std::string buildTimeoutConfig(const SessionTimeout& config);

// Check if timeout is enabled
bool isTimeoutEnabled(const SessionTimeout& config);

} // namespace progressive
