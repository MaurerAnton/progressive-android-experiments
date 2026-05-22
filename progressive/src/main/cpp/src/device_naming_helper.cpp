#include "progressive/device_naming_helper.hpp"
#include <sstream>

namespace progressive {

std::string generateDeviceName(const std::string& mfr, const std::string& model,
                                 const std::string& os, const std::string& osVer) {
    std::ostringstream os_name;
    if (!mfr.empty()) os_name << mfr << " ";
    if (!model.empty()) os_name << model;
    if (os_name.str().empty()) {
        os_name << os << " " << osVer;
    }
    return os_name.str();
}

std::string parseDeviceNameFromUserAgent(const std::string& ua) {
    auto openParen = ua.find('(');
    auto closeParen = ua.rfind(')');
    if (openParen == std::string::npos || closeParen == std::string::npos) return ua;
    
    std::string deviceInfo = ua.substr(openParen + 1, closeParen - openParen - 1);
    auto semi = deviceInfo.find(';');
    if (semi != std::string::npos) {
        std::string mfrModel = deviceInfo.substr(0, semi);
        // Trim
        while (!mfrModel.empty() && mfrModel.back() == ' ') mfrModel.pop_back();
        while (!mfrModel.empty() && mfrModel.front() == ' ') mfrModel = mfrModel.substr(1);
        return mfrModel;
    }
    return deviceInfo;
}

std::string formatDeviceDisplayName(const std::string& name, const std::string& os,
                                      const std::string& ver) {
    std::ostringstream os_name;
    os_name << name;
    if (!os.empty()) os_name << " [" << os;
    if (!ver.empty()) os_name << " " << ver;
    if (!os.empty()) os_name << "]";
    return os_name.str();
}

bool isDefaultDeviceName(const std::string& name) {
    return name.find("Progressive") != std::string::npos ||
           name.find("Android") != std::string::npos ||
           name.find("sdk_gphone") != std::string::npos;
}

std::string truncateDeviceName(const std::string& name, int maxLen) {
    if ((int)name.size() <= maxLen) return name;
    return name.substr(0, maxLen - 3) + "...";
}

} // namespace progressive
