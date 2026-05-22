#pragma once
#include <string>
#include <vector>

namespace progressive {

struct LinkPreviewData {
    std::string url;
    std::string title;
    std::string description;
    std::string imageUrl;
    std::string siteName;
    int imageWidth = 0;
    int imageHeight = 0;
    bool hasPreview = false;
};

struct ExtractedLink {
    std::string url;
    int startIndex = 0;
    int endIndex = 0;
};

// Extract all URLs from text
std::vector<ExtractedLink> extractUrls(const std::string& text);

// Parse OpenGraph HTML meta tags
LinkPreviewData parseOpenGraph(const std::string& html);

// Parse oEmbed JSON response
LinkPreviewData parseOEmbed(const std::string& json);

// Format link preview for display in timeline
std::string formatLinkPreview(const LinkPreviewData& data);

// Build Matrix URL preview event content
std::string buildUrlPreviewContent(const std::string& url, const LinkPreviewData& data);

// Check if URL is an image (for inline image preview)
bool isImageUrl(const std::string& url);
std::string getDomainFromUrl(const std::string& url);

} // namespace progressive
