#include "progressive/url_preview_formatter.hpp"
#include <sstream>

namespace progressive {
std::string extractFirstUrl(const std::string& text) {
    auto p = text.find("http"); if (p == std::string::npos) return "";
    size_t end = p; while (end < text.size() && text[end] != ' ' && text[end] != '\n') end++;
    return text.substr(p, end - p);
}
UrlPreviewCard parseUrlPreviewCard(const std::string& json) {
    UrlPreviewCard c;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\""); if (p == std::string::npos) return "";
        p += key.size() + 4; auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    c.url = extract("url"); c.title = extract("og:title"); c.description = extract("og:description");
    c.imageUrl = extract("og:image"); c.siteName = extract("og:site_name"); c.hasPreview = !c.title.empty();
    return c;
}
std::string formatUrlPreviewCard(const UrlPreviewCard& c) {
    std::ostringstream os; os << c.title; if (!c.siteName.empty()) os << " (" << c.siteName << ")"; return os.str();
}
bool shouldShowUrlPreview(const std::string& url) { return url.size() > 12; }
} // namespace progressive
