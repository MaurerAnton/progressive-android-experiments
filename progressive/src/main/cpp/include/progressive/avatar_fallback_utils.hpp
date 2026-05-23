#pragma once
#include <string>
namespace progressive {
std::string computeInitials(const std::string& name, int maxChars=2);
std::string getAvatarColor(const std::string& id);
int getAvatarSizeDp(bool isDirect, bool isSmall=false);
}
