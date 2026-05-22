#include "progressive/spoiler_utils.hpp"
#include <sstream>

namespace progressive {

SpoilerMatch parseSpoilerFromHtml(const std::string& html) {
    SpoilerMatch m;
    auto spoilerPos = html.find("data-mx-spoiler");
    if (spoilerPos == std::string::npos) return m;
    
    auto reasonStart = html.find('"', spoilerPos + 16);
    if (reasonStart != std::string::npos) {
        reasonStart++;
        auto reasonEnd = html.find('"', reasonStart);
        if (reasonEnd != std::string::npos)
            m.reason = html.substr(reasonStart, reasonEnd - reasonStart);
    }
    return m;
}

std::string buildSpoilerHtml(const std::string& content, const std::string& reason) {
    std::ostringstream os;
    os << "<span data-mx-spoiler";
    if (!reason.empty()) os << "=\"" << reason << "\"";
    os << ">" << content << "</span>";
    return os.str();
}

std::string extractSpoilerReason(const std::string& html) {
    return parseSpoilerFromHtml(html).reason;
}

bool hasSpoiler(const std::string& html) {
    return html.find("data-mx-spoiler") != std::string::npos;
}

std::string formatSpoilerPlain(const std::string& content, const std::string& reason) {
    std::string label = reason.empty() ? "spoiler" : reason;
    return "(" + label + ") " + content;
}

} // namespace progressive
