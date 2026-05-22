#pragma once
#include <string>
#include <vector>

namespace progressive {

struct HighlightRule {
    std::string keyword;        // word to highlight
    std::string color;          // hex color or name
    bool wholeWord = false;
    bool caseSensitive = false;
    bool enabled = true;
};

// Check if text contains any highlight keywords
bool hasHighlight(const std::string& text, const std::vector<HighlightRule>& rules);

// Format text with HTML highlight spans around matching keywords
std::string applyHighlights(const std::string& text, const std::vector<HighlightRule>& rules);

// Build highlight push rule from keywords (for Matrix push rules)
std::string buildHighlightPushRule(const std::vector<HighlightRule>& rules, const std::string& ruleId);

// Parse keywords from push rule JSON
std::vector<HighlightRule> parseHighlightRules(const std::string& pushRuleJson);

// Format notification body with highlighted sender name
std::string formatHighlightedNotification(const std::string& senderName,
                                            const std::string& body,
                                            bool isHighlight);

} // namespace progressive
