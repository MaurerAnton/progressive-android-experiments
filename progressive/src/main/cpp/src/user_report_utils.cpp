#include "progressive/user_report_utils.hpp"
#include <sstream>

namespace progressive {

ReportReason reportReasonFromString(const std::string& s) {
    if (s == "m.spam") return ReportReason::SPAM;
    if (s == "m.inappropriate") return ReportReason::INAPPROPRIATE;
    if (s == "m.custom") return ReportReason::CUSTOM;
    return ReportReason::UNKNOWN;
}

std::string reportReasonToString(ReportReason r) {
    switch (r) {
        case ReportReason::SPAM: return "m.spam";
        case ReportReason::INAPPROPRIATE: return "m.inappropriate";
        case ReportReason::CUSTOM: return "m.custom";
        default: return "unknown";
    }
}

std::string buildReportContent(const std::string& eventId, ReportReason reason,
                                 const std::string& customReason) {
    std::ostringstream os;
    os << R"({"reason":")" << reportReasonToString(reason) << R"(")";
    if (reason == ReportReason::CUSTOM && !customReason.empty())
        os << R"(,"reason_text":")" << customReason << R"(")";
    os << R"(,"event_id":")" << eventId << R"(")";
    os << "}";
    return os.str();
}

std::string buildReportServerNotice(const std::string& reason) {
    return R"({"reason":")" + reason + R"("})";
}

UserReport parseReportResponse(const std::string& json) {
    UserReport r;
    auto scorePos = json.find("\"score\":");
    if (scorePos != std::string::npos) {
        scorePos += 8;
        try { r.score = std::stoll(json.substr(scorePos)); } catch(...) {}
    }
    return r;
}

std::string formatReportReason(ReportReason r) {
    switch (r) {
        case ReportReason::SPAM: return "Spam";
        case ReportReason::INAPPROPRIATE: return "Inappropriate content";
        case ReportReason::CUSTOM: return "Custom report";
        default: return "Unknown";
    }
}

} // namespace progressive
