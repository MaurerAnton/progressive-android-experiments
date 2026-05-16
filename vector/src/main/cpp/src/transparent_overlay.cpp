#include "progressive/transparent_overlay.hpp"
#include <sstream>
#include <cmath>

namespace progressive {

// ====== Constructor ======

TransparentOverlayEngine::TransparentOverlayEngine() {}

void TransparentOverlayEngine::setConfig(const TransparentOverlayConfig& config) { config_ = config; }
TransparentOverlayConfig TransparentOverlayEngine::getConfig() const { return config_; }

// ====== Helpers ======

bool TransparentOverlayEngine::isFingerStill(double startX, double startY,
                                              double currentX, double currentY) const {
    double dx = currentX - startX;
    double dy = currentY - startY;
    double dist = std::sqrt(dx * dx + dy * dy);
    return dist <= config_.oneFingerMovePx;
}

bool TransparentOverlayEngine::isTwoFingerHoldElapsed(int64_t nowNs) const {
    if (!state_.twoFingerWaiting) return false;
    int64_t elapsedMs = (nowNs - state_.twoFingerHoldSinceNs) / 1000000LL;
    return elapsedMs >= config_.twoFingerHoldMs;
}

bool TransparentOverlayEngine::isForegroundExpired(int64_t nowNs) const {
    if (!state_.backgroundIsForeground) return false;
    if (state_.foregroundUntilNs == 0) return false;
    return nowNs >= state_.foregroundUntilNs;
}

// ====== Touch Events ======

TouchAction TransparentOverlayEngine::touchDown(double x, double y, int pointerId, int64_t timeNs) {
    state_.currentPointerCount++;

    if (!primaryIsDown_) {
        // First finger down
        primaryX_ = x;
        primaryY_ = y;
        primaryStartX_ = x;
        primaryStartY_ = y;
        primaryDownTimeNs_ = timeNs;
        primaryIsDown_ = true;
        swipeStartTimeNs_ = timeNs;
        swipeTotalDx_ = 0;
        swipeTotalDy_ = 0;

        // If background is in foreground, check if this is a tap to extend
        if (state_.backgroundIsForeground && config_.enableTwoFingerSwitch) {
            state_.foregroundUntilNs = timeNs + config_.foregroundExtendedMs * 1000000LL;
            return TouchAction::EXTEND_TIMER;
        }

        // Start tracking one-finger hold
        if (config_.enableOneFingerPassThrough && !state_.backgroundIsForeground) {
            state_.gesture = GestureType::ONE_FINGER_STILL;
            state_.oneFingerStillSinceNs = timeNs;
            state_.firstFingerX = x;
            state_.firstFingerY = y;
        }

        return TouchAction::NONE;
    }

    // Second finger down
    if (!secondaryIsDown_ && config_.enableTwoFingerSwitch) {
        secondaryX_ = x;
        secondaryY_ = y;
        secondaryDownTimeNs_ = timeNs;
        secondaryIsDown_ = true;

        if (state_.backgroundIsForeground) {
            // Two fingers while background is foreground → quick return
            state_.twoFingerHoldSinceNs = timeNs;
            state_.twoFingerWaiting = true;
            if (config_.quickReturnHoldMs == 0) {
                // Immediate return
                state_.backgroundIsForeground = false;
                state_.gesture = GestureType::RETURN_TRANSITION;
                return TouchAction::RETURN_TO_FOREGROUND;
            }
        } else if (state_.gesture == GestureType::ONE_FINGER_ARMED) {
            // Start two-finger hold for switch
            state_.twoFingerHoldSinceNs = timeNs;
            state_.twoFingerWaiting = true;
            state_.gesture = GestureType::TWO_FINGER_HOLD;
        }

        return TouchAction::NONE;
    }

    return TouchAction::NONE;
}

TouchAction TransparentOverlayEngine::touchMove(double x, double y, int pointerId, int64_t timeNs) {
    if (pointerId == 0 && primaryIsDown_) {
        primaryX_ = x;
        primaryY_ = y;
        swipeTotalDx_ = x - primaryStartX_;
        swipeTotalDy_ = y - primaryStartY_;
    } else if (pointerId == 1 && secondaryIsDown_) {
        secondaryX_ = x;
        secondaryY_ = y;
    }

    // Check if one-finger hold is still valid
    if (state_.gesture == GestureType::ONE_FINGER_STILL) {
        if (isFingerStill(primaryStartX_, primaryStartY_, primaryX_, primaryY_)) {
            int64_t elapsedMs = (timeNs - state_.oneFingerStillSinceNs) / 1000000LL;
            if (elapsedMs >= config_.oneFingerHoldMs) {
                state_.gesture = GestureType::ONE_FINGER_ARMED;
                state_.oneFingerArmed = true;
            }
        } else {
            // Finger moved too much — reset
            state_.gesture = GestureType::IDLE;
            state_.oneFingerArmed = false;
        }
    }

    // Check if two-finger hold elapsed (for foreground switch)
    if (state_.twoFingerWaiting && isTwoFingerHoldElapsed(timeNs)) {
        state_.twoFingerWaiting = false;

        if (state_.backgroundIsForeground) {
            // Return to foreground
            state_.backgroundIsForeground = false;
            state_.foregroundUntilNs = 0;
            state_.gesture = GestureType::RETURN_TRANSITION;
            return TouchAction::RETURN_TO_FOREGROUND;
        } else {
            // Bring background to foreground
            state_.backgroundIsForeground = true;
            state_.foregroundUntilNs = timeNs + config_.foregroundDurationMs * 1000000LL;
            state_.gesture = GestureType::FOREGROUND_TRANSITION;
            return TouchAction::BRING_BACKGROUND_FOREGROUND;
        }
    }

    // One-finger armed + second finger moving → route to background
    if (state_.oneFingerArmed && secondaryIsDown_ && config_.enableOneFingerPassThrough) {
        return TouchAction::ROUTE_TO_BACKGROUND;
    }

    return TouchAction::NONE;
}

TouchAction TransparentOverlayEngine::touchUp(int pointerId, int64_t timeNs) {
    state_.currentPointerCount--;

    if (pointerId == 0) {
        primaryIsDown_ = false;

        // Check for swipe-down to return (when background is foreground)
        if (state_.backgroundIsForeground && config_.enableSwipeToReturn) {
            int64_t swipeDurationNs = timeNs - swipeStartTimeNs_;
            double swipeVelocity = std::abs(swipeTotalDy_) / (static_cast<double>(swipeDurationNs) / 1e9);
            if (swipeVelocity >= config_.swipeVelocityPxPerSec &&
                std::abs(swipeTotalDy_) >= config_.swipeDistancePx &&
                swipeTotalDy_ > 0) { // Swipe down
                state_.backgroundIsForeground = false;
                state_.foregroundUntilNs = 0;
                return TouchAction::RETURN_TO_FOREGROUND;
            }
        }

        // Reset one-finger hold state
        if (state_.gesture == GestureType::ONE_FINGER_STILL ||
            state_.gesture == GestureType::ONE_FINGER_ARMED) {
            state_.gesture = GestureType::IDLE;
            state_.oneFingerArmed = false;
        }
    }

    if (pointerId == 1) {
        secondaryIsDown_ = false;
        state_.twoFingerWaiting = false;
    }

    if (state_.currentPointerCount <= 0) {
        state_.currentPointerCount = 0;
        state_.twoFingerWaiting = false;
    }

    return TouchAction::NONE;
}

TouchAction TransparentOverlayEngine::backPressed(int64_t timeNs) {
    if (state_.backgroundIsForeground && config_.enableBackButton) {
        state_.backgroundIsForeground = false;
        state_.foregroundUntilNs = 0;
        state_.gesture = GestureType::RETURN_TRANSITION;
        return TouchAction::RETURN_TO_FOREGROUND;
    }
    return TouchAction::NONE;
}

// ====== Timer Tick ======

TouchAction TransparentOverlayEngine::timerTick(int64_t timeNs) {
    // Check if foreground display has expired
    if (isForegroundExpired(timeNs) && state_.backgroundIsForeground) {
        state_.backgroundIsForeground = false;
        state_.foregroundUntilNs = 0;
        state_.gesture = GestureType::RETURN_TRANSITION;
        return TouchAction::RETURN_TO_FOREGROUND;
    }

    // Check two-finger hold timer
    if (state_.twoFingerWaiting && isTwoFingerHoldElapsed(timeNs)) {
        state_.twoFingerWaiting = false;

        if (state_.backgroundIsForeground) {
            state_.backgroundIsForeground = false;
            state_.foregroundUntilNs = 0;
            state_.gesture = GestureType::RETURN_TRANSITION;
            return TouchAction::RETURN_TO_FOREGROUND;
        } else {
            state_.backgroundIsForeground = true;
            state_.foregroundUntilNs = timeNs + config_.foregroundDurationMs * 1000000LL;
            state_.gesture = GestureType::FOREGROUND_TRANSITION;
            return TouchAction::BRING_BACKGROUND_FOREGROUND;
        }
    }

    return TouchAction::NONE;
}

// ====== State ======

OverlayState TransparentOverlayEngine::getState() const {
    return state_;
}

TouchAction TransparentOverlayEngine::getTouchAction(double x, double y, int pointerCount, int64_t timeNs) const {
    if (state_.backgroundIsForeground) return TouchAction::NONE; // User is interacting with background

    // One-finger armed + multiple pointers → route to background
    if (state_.oneFingerArmed && pointerCount >= 2 && config_.enableOneFingerPassThrough) {
        return TouchAction::ROUTE_TO_BACKGROUND;
    }

    return TouchAction::NONE;
}

// ====== Serialization ======

std::string TransparentOverlayEngine::stateToJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"gesture":)" << static_cast<int>(state_.gesture)
       << R"(,"bg_is_fg":)" << (state_.backgroundIsForeground ? "true" : "false")
       << R"(,"one_finger_armed":)" << (state_.oneFingerArmed ? "true" : "false")
       << R"(,"two_finger_waiting":)" << (state_.twoFingerWaiting ? "true" : "false")
       << R"(,"fg_until_ms":)" << (state_.foregroundUntilNs / 1000000LL)
       << R"(,"pointers":)" << state_.currentPointerCount
       << "}";
    return os.str();
}

std::string TransparentOverlayEngine::configToJson() const {
    std::ostringstream os;
    os << R"({"one_finger_hold_ms":)" << config_.oneFingerHoldMs
       << R"(,"one_finger_move_px":)" << config_.oneFingerMovePx
       << R"(,"two_finger_hold_ms":)" << config_.twoFingerHoldMs
       << R"(,"fg_duration_ms":)" << config_.foregroundDurationMs
       << R"(,"fg_extended_ms":)" << config_.foregroundExtendedMs
       << R"(,"return_ms":)" << config_.returnDurationMs
       << R"(,"quick_return_ms":)" << config_.quickReturnHoldMs
       << R"(,"enable_one_finger":)" << (config_.enableOneFingerPassThrough ? "true" : "false")
       << R"(,"enable_two_finger":)" << (config_.enableTwoFingerSwitch ? "true" : "false")
       << R"(,"enable_back":)" << (config_.enableBackButton ? "true" : "false")
       << R"(,"enable_swipe":)" << (config_.enableSwipeToReturn ? "true" : "false")
       << R"(,"swipe_velocity":)" << config_.swipeVelocityPxPerSec
       << R"(,"swipe_distance":)" << config_.swipeDistancePx
       << "}";
    return os.str();
}

} // namespace progressive
