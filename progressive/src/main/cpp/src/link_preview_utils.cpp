#include "progressive/link_preview_utils.hpp"
#include <sstream>

namespace progressive {

std::vector<ExtractedLink> extractUrls(const std::string& text) {
    std::vector<ExtractedLink> links;
    size_t pos = 0;
    while (pos < text.size()) {
        auto http = text.find("http", pos);
        if (http == std::string::npos) break;
        size_t end = http;
        while (end < text.size() && text[end] != ' ' && text[end] != '\n' &&
               text[end] != '"' && text[end] != '\'' && text[end] != '>' &&
               text[end] != '<' && text[end] != ')') end++;
        std::string url = text.substr(http, end - http);
        if (url.size() > 10) links.push_back({url, (int)http, (int)end});
        pos = end;
    }
    return links;
}

static std::string extractMeta(const std::string& html, const std::string& property) {
    auto pos = html.find(property);
    if (pos == std::string::npos) return "";
    auto content = html.find("content=\"", pos);
    if (content == std::string::npos) content = html.find("content='", pos);
    if (content == std::string::npos) return "";
    content += 9;
    auto end = html.find('"', content);
    if (end == std::string::npos) end = html.find('\'', content);
    if (end == std::string::npos) return "";
    return html.substr(content, end - content);
}

LinkPreviewData parseOpenGraph(const std::string& html) {
    LinkPreviewData d;
    d.title = extractMeta(html, "og:title");
    if (d.title.empty()) {
        auto titlePos = html.find("<title>");
        if (titlePos != std::string::npos) {
            titlePos += 7;
            auto end = html.find("</title>", titlePos);
            if (end != std::string::npos) d.title = html.substr(titlePos, end - titlePos);
        }
    }
    d.description = extractMeta(html, "og:description");
    d.imageUrl = extractMeta(html, "og:image");
    d.siteName = extractMeta(html, "og:site_name");
    d.hasPreview = !d.title.empty();
    return d;
}

LinkPreviewData parseOEmbed(const std::string& json) {
    LinkPreviewData d;
    auto title = json.find("\"title\":\"");
    if (title != std::string::npos) {
        title += 9;
        auto end = json.find('"', title);
        if (end != std::string::npos) d.title = json.substr(title, end - title);
    }
    auto thumb = json.find("\"thumbnail_url\":\"");
    if (thumb != std::string::npos) {
        thumb += 17;
        auto end = json.find('"', thumb);
        if (end != std::string::npos) d.imageUrl = json.substr(thumb, end - thumb);
    }
    d.hasPreview = !d.title.empty();
    return d;
}

std::string formatLinkPreview(const LinkPreviewData& data) {
    if (!data.hasPreview) return data.url;
    std::ostringstream os;
    os << data.siteName << ": " << data.title;
    return os.str();
}

std::string buildUrlPreviewContent(const std::string& url, const LinkPreviewData& data) {
    std::ostringstream os;
    os << R"({"url":")" << url << R"(")";
    if (data.hasPreview) {
        os << R"(,"og:title":")" << data.title << R"(")";
        if (!data.description.empty()) os << R"(,"og:description":")" << data.description << R"(")";
        if (!data.imageUrl.empty()) os << R"(,"og:image":")" << data.imageUrl << R"(")";
    }
    os << "}";
    return os.str();
}

bool isImageUrl(const std::string& url) {
    std::string lower;
    std::transform(url.begin(), url.end(), std::back_inserter(lower), ::tolower);
    return lower.find(".jpg") != std::string::npos ||
           lower.find(".jpeg") != std::string::npos ||
           lower.find(".png") != std::string::npos ||
           lower.find(".gif") != std::string::npos ||
           lower.find(".webp") != std::string::npos ||
           lower.find(".svg") != std::string::npos;
}

std::string getDomainFromUrl(const std::string& url) {
    auto start = url.find("://");
    if (start == std::string::npos) return url;
    start += 3;
    auto end = url.find('/', start);
    if (end == std::string::npos) end = url.size();
    std::string domain = url.substr(start, end - start);
    if (domain.compare(0, 4, "www.") == 0) domain = domain.substr(4);
    return domain;
}

} // namespace progressive
