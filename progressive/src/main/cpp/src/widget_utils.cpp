#include "progressive/widget_utils.hpp"
#include <sstream>

namespace progressive {

WidgetInfo parseWidget(const std::string& sk, const std::string& json) {
    WidgetInfo w;
    w.widgetId = sk;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    w.type = extract("type");
    w.name = extract("name");
    w.url = extract("url");
    w.creatorUserId = extract("creatorUserId");
    w.avatarUrl = extract("avatar_url");
    w.waitForIframeLoad = json.find("\"waitForIframeLoad\":true") != std::string::npos;
    return w;
}

std::string buildWidgetContent(const WidgetInfo& w) {
    std::ostringstream os;
    os << R"({"type":")" << w.type << R"(")";
    if (!w.name.empty()) os << R"(,"name":")" << w.name << R"(")";
    os << R"(,"url":")" << w.url << R"(")";
    if (!w.creatorUserId.empty()) os << R"(,"creatorUserId":")" << w.creatorUserId << R"(")";
    os << R"(,"waitForIframeLoad":)" << (w.waitForIframeLoad ? "true" : "false");
    os << "}";
    return os.str();
}

WidgetAction parseWidgetAction(const std::string& json) {
    WidgetAction a;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    a.action = extract("action");
    a.widgetId = extract("widgetId");
    a.requestId = extract("requestId");
    return a;
}

std::string buildWidgetActionResponse(const WidgetAction& a, const std::string& result) {
    std::ostringstream os;
    os << R"({"requestId":")" << a.requestId << R"(")";
    os << R"(,"result":)" << result;
    os << "}";
    return os.str();
}

std::string formatWidgetInfo(const WidgetInfo& w) {
    return w.name + " (" + w.type + ")";
}

bool isValidWidgetUrl(const std::string& url) {
    return url.compare(0, 8, "https://") == 0;
}

} // namespace progressive
