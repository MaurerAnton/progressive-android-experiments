#ifndef PROGRESSIVE_PUSH_RULES_HPP
#define PROGRESSIVE_PUSH_RULES_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Push Rule Condition Parser ----
// Replaces the SDK's limited condition parser that falls back to "UNSUPPORTED".
// Handles ALL known Matrix push rule condition types with human-readable descriptions.

struct PushCondition {
    std::string kind;              // "event_match", "contains_display_name", etc.
    std::string key;               // "content.body", "room_id", etc.
    std::string pattern;           // match pattern or value
    std::string description;       // human-readable: "Message contains 'hello'"
    bool isSupported = true;       // always true for our parser
};

struct PushRule {
    std::string ruleId;
    std::string kind;              // "override", "underride", "content", "room", "sender"
    bool enabled = true;
    std::vector<PushCondition> conditions;
    std::vector<std::string> actions; // "notify", "dont_notify", "coalesce"
    bool shouldNotify = true;
    bool shouldHighlight = false;
    std::string soundName;
};

struct PushRuleSet {
    std::vector<PushRule> rules;
    int totalRules = 0;
    int enabledRules = 0;
};

// Parse push rules from the /pushrules API response.
PushRuleSet parsePushRules(const std::string& apiResponseJson);

// Parse a single condition from JSON and produce a human-readable description.
PushCondition parsePushCondition(const std::string& conditionJson);

// Build a human-readable description for a condition.
std::string describePushCondition(const PushCondition& condition);

// Format all conditions of a rule as text for UI display.
std::string formatPushRuleConditions(const std::vector<PushCondition>& conditions);

// Format a push rule for UI item display.
std::string formatPushRuleItem(const PushRule& rule);

// Check if a push rule matches the standard types known to Matrix.
bool isKnownPushRuleKind(const std::string& kind);

// Get a localized description for a push rule kind.
std::string getRuleKindDescription(const std::string& kind, bool enabled);

// ---- MSC3061 Shared History Detection ----

// Check if a room key was shared under MSC3061 (world_readable or shared visibility).
bool isMsc3061SharedKey(const std::string& roomKeyContentJson);

// Format MSC3061 key sharing status for UI display.
std::string formatMsc3061Status(bool isShared, const std::string& visibilitySetting);

// Check room visibility setting for history sharing eligibility.
bool canShareHistory(const std::string& roomVisibility);

} // namespace progressive

#endif // PROGRESSIVE_PUSH_RULES_HPP
