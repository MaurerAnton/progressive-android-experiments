#include "progressive/markdown_utils.hpp"
#include <sstream>

namespace progressive {

std::string stripMarkdown(const std::string& text) {
    std::string result;
    bool inBold = false, inItalic = false, inCode = false, inLink = false;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (!inCode && i + 1 < text.size() && text[i] == '*' && text[i+1] == '*') {
            inBold = !inBold; i++; continue;
        }
        if (!inCode && c == '*' && !inBold) { inItalic = !inItalic; continue; }
        if (c == '`') { inCode = !inCode; continue; }
        if (!inCode && c == '[') { inLink = true; continue; }
        if (inLink && c == ']') { inLink = false; continue; }
        if (!inLink) result += c;
    }
    return result;
}

std::string markdownToHtml(const std::string& text) {
    std::ostringstream os;
    bool inBold = false, inItalic = false, inCode = false;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (!inCode && i + 1 < text.size() && text[i] == '*' && text[i+1] == '*') {
            os << (inBold ? "</b>" : "<b>"); inBold = !inBold; i++; continue;
        }
        if (!inCode && c == '*' && !inBold) {
            os << (inItalic ? "</i>" : "<i>"); inItalic = !inItalic; continue;
        }
        if (c == '`') { os << (inCode ? "</code>" : "<code>"); inCode = !inCode; continue; }
        if (c == '\n') os << "<br/>";
        else os << c;
    }
    if (inCode) os << "</code>";
    if (inBold) os << "</b>";
    if (inItalic) os << "</i>";
    return os.str();
}

std::vector<std::string> extractLinks(const std::string& text) {
    std::vector<std::string> links;
    size_t pos = 0;
    while (pos < text.size()) {
        auto http = text.find("http", pos);
        if (http == std::string::npos) break;
        size_t end = http;
        while (end < text.size() && text[end] != ' ' && text[end] != '\n' &&
               text[end] != '"' && text[end] != ')' && text[end] != '>') end++;
        links.push_back(text.substr(http, end - http));
        pos = end;
    }
    return links;
}

int visibleCharCount(const std::string& text) {
    return (int)stripMarkdown(text).size();
}

std::string truncateMarkdown(const std::string& text, int maxVisibleChars) {
    std::string plain = stripMarkdown(text);
    if ((int)plain.size() <= maxVisibleChars) return text;
    
    // Walk through original text counting visible chars
    std::string result;
    int visible = 0;
    bool skip = false;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (c == '*' || c == '`' || c == '[' || c == ']' || c == '_') {
            result += c; continue;
        }
        if (!skip) visible++;
        result += c;
        if (visible >= maxVisibleChars) {
            result += "...";
            break;
        }
    }
    return result;
}

std::vector<MarkdownSegment> parseMarkdown(const std::string& text) {
    std::vector<MarkdownSegment> segs;
    MarkdownSegment cur;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '*' && i + 1 < text.size() && text[i+1] == '*') {
            if (!cur.text.empty()) { segs.push_back(cur); cur = {}; }
            cur.type = MarkdownSegment::BOLD;
            i++; continue;
        }
        if (text[i] == '*' && cur.type != MarkdownSegment::BOLD) {
            if (!cur.text.empty()) { segs.push_back(cur); cur = {}; }
            cur.type = MarkdownSegment::ITALIC;
            i++; continue;
        }
        cur.text += text[i];
    }
    if (!cur.text.empty()) segs.push_back(cur);
    return segs;
}

std::string segmentsToPlainText(const std::vector<MarkdownSegment>& segs) {
    std::string result;
    for (const auto& s : segs) result += s.text;
    return result;
}

} // namespace progressive
