#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct MarkdownSegment {
    enum Type { TEXT, BOLD, ITALIC, CODE, LINK, HEADING, QUOTE, LIST_ITEM };
    Type type = TEXT;
    std::string text;
    std::string url;       // for links
    int headingLevel = 0;  // 1-6 for headings
    int listLevel = 0;     // nesting level for lists
};

// Parse markdown text into structured segments
std::vector<MarkdownSegment> parseMarkdown(const std::string& text);

// Convert segments back to plain text
std::string segmentsToPlainText(const std::vector<MarkdownSegment>& segments);

// Strip all markdown formatting
std::string stripMarkdown(const std::string& text);

// Convert markdown to HTML
std::string markdownToHtml(const std::string& text);

// Extract all links from markdown text
std::vector<std::string> extractLinks(const std::string& text);

// Count characters excluding markdown syntax
int visibleCharCount(const std::string& text);

// Truncate markdown to N visible characters
std::string truncateMarkdown(const std::string& text, int maxVisibleChars);

} // namespace progressive
