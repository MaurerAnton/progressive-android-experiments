#pragma once
#include <string>
namespace progressive {
std::string getDefaultNotificationSound();
std::string getHighlightNotificationSound();
std::string buildSoundTweak(const std::string& soundUri);
}
