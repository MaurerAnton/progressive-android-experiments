#include "progressive/toast_notification_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildToastMessage(const std::string& title, const std::string& body) { return title + ": " + body; }
std::string buildDelayedToast(int delay, const std::string& msg) { return msg + " (in " + std::to_string(delay/1000) + "s)"; }
std::string formatToastTime(int delay) { return std::to_string(delay/1000) + "s"; }
}
