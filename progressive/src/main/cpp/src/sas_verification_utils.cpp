#include "progressive/sas_verification_utils.hpp"
#include <sstream>

namespace progressive {

SasEmoji getSasEmoji(int index) {
    return SAS_EMOJIS[index % SAS_EMOJIS.size()];
}

std::vector<int> getSasDecimals(const std::string& bytes, int count) {
    std::vector<int> decimals;
    for (size_t i = 0; i < bytes.size() && (int)decimals.size() < count * 3; i += 2) {
        if (i + 1 < bytes.size()) {
            int val = ((unsigned char)bytes[i] << 8) | (unsigned char)bytes[i + 1];
            decimals.push_back(val % 10000);
        }
    }
    return decimals;
}

std::string formatSasEmojis(const std::vector<int>& indices) {
    std::ostringstream os;
    for (size_t i = 0; i < indices.size(); i++) {
        if (i > 0 && i % 7 == 0) os << "\n";
        os << getSasEmoji(indices[i]).emoji << " ";
    }
    return os.str();
}

std::string formatSasDecimals(const std::vector<int>& decimals) {
    std::ostringstream os;
    for (size_t i = 0; i < decimals.size(); i++) {
        if (i > 0) os << "  ";
        os << decimals[i];
    }
    return os.str();
}

std::string extractSasBytes(const std::string& json) {
    auto macPos = json.find("\"mac\":\"");
    if (macPos != std::string::npos) {
        macPos += 7;
        auto macEnd = json.find('"', macPos);
        if (macEnd != std::string::npos) return json.substr(macPos, macEnd - macPos);
    }
    return "";
}

} // namespace progressive
