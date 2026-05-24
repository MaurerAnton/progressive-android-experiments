#include "progressive/url_preview_parser.hpp"
#include <sstream>

namespace progressive {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    if (e == std::string::npos) return "";
    return json.substr(p, e - p);
}
static int extractInt(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":");
    if (p == std::string::npos) return 0;
    p += key.size() + 2;
    try { return std::stoi(json.substr(p)); } catch(...) { return 0; }
}

std::string buildPreviewRequest(const std::string& url, int64_t ts) {
    std::ostringstream os;
    os << R"({"url":")" << url << R"(")";
    if (ts > 0) os << R"(,"ts":)" << ts;
    os << "}";
    return os.str();
}

UrlPreview parseUrlPreview(const std::string& json) {
    UrlPreview p;
    p.url = extractField(json, "url");
    p.title = extractField(json, "og:title");
    if (p.title.empty()) p.title = extractField(json, "title");
    p.description = extractField(json, "og:description");
    if (p.description.empty()) p.description = extractField(json, "description");
    p.imageUrl = extractField(json, "og:image");
    p.siteName = extractField(json, "og:site_name");
    p.imageWidth = extractInt(json, "og:image:width");
    p.imageHeight = extractInt(json, "og:image:height");
    p.contentLength = extractInt(json, "matrix:image:size");
    return p;
}

bool isUrlPreviewable(const std::string& url) {
    return url.compare(0, 7, "http://") == 0 || url.compare(0, 8, "https://") == 0;
}

std::string extractPreviewableUrl(const std::string& text) {
    size_t pos = 0;
    while (pos < text.size()) {
        auto http = text.find("http", pos);
        if (http == std::string::npos) break;
        size_t end = http;
        while (end < text.size() && text[end] != ' ' && text[end] != '\n' &&
               text[end] != '"' && text[end] != '<' && text[end] != ')') end++;
        std::string url = text.substr(http, end - http);
        if (url.size() > 12) return url;
        pos = end;
    }
    return "";
}

std::string formatUrlPreviewHtml(const UrlPreview& p) {
    std::ostringstream os;
    os << "<div class=\"url-preview\">";
    if (!p.imageUrl.empty()) {
        os << "<img src=\"" << p.imageUrl << "\" style=\"max-width:100%;max-height:200px\"/>";
    }
    os << "<div class=\"url-preview-text\">";
    os << "<strong>" << p.title << "</strong><br/>";
    if (!p.description.empty()) os << p.description << "<br/>";
    if (!p.siteName.empty()) os << "<small>" << p.siteName << "</small>";
    os << "</div></div>";
    return os.str();
}

} // namespace progressive
