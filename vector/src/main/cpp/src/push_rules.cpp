#include "progressive/push_rules.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

PushCondition parsePushCondition(const std::string& conditionJson) {
    PushCondition cond;
    cond.kind = parseJsonStringValue(conditionJson, "kind");
    cond.key  = parseJsonStringValue(conditionJson, "key");
    cond.pattern = parseJsonStringValue(conditionJson, "pattern");

    // Alternative fields
    if (cond.pattern.empty()) cond.pattern = parseJsonStringValue(conditionJson, "is");
    if (cond.key.empty()) cond.key = parseJsonStringValue(conditionJson, "is");

    cond.description = describePushCondition(cond);
    cond.isSupported = true; // our parser handles everything
    return cond;
}

std::string describePushCondition(const PushCondition& cond) {
    if (cond.kind == "event_match") {
        if (cond.key == "content.body") {
            return "Message contains \"" + cond.pattern + "\"";
        }
        if (cond.key == "type") {
            return "Event type is \"" + cond.pattern + "\"";
        }
        if (cond.key == "sender") {
            return "From \"" + cond.pattern + "\"";
        }
        if (cond.key == "room_id") {
            return "Room ID is \"" + cond.pattern + "\"";
        }
        return "Event field \"" + cond.key + "\" matches \"" + cond.pattern + "\"";
    }

    if (cond.kind == "contains_display_name") {
        return "Message mentions you by name";
    }

    if (cond.kind == "room_member_count") {
        if (!cond.pattern.empty()) {
            return "Room has " + cond.pattern + " or fewer members";
        }
        auto count = parseJsonStringValue(cond.pattern.empty() ? "" : "", "is");
        return "Room member count condition";
    }

    if (cond.kind == "sender_notification_permission") {
        return "Sender has notification permission (\"" + cond.key + "\")";
    }

    if (cond.kind == "org.matrix.msc3931.push_rule_condition") {
        return "Advanced push condition (MSC3931)";
    }

    if (cond.kind == "org.matrix.msc3061.shared_history") {
        return "Shared history key (MSC3061)";
    }

    // Fallback — we understand it even if custom
    std::ostringstream desc;
    desc << "Condition: " << cond.kind;
    if (!cond.key.empty()) desc << " on \"" << cond.key << "\"";
    if (!cond.pattern.empty()) desc << " = \"" << cond.pattern << "\"";
    return desc.str();
}

std::string formatPushRuleConditions(const std::vector<PushCondition>& conditions) {
    if (conditions.empty()) return "No conditions";
    std::ostringstream out;
    for (size_t i = 0; i < conditions.size(); ++i) {
        if (i > 0) out << " AND ";
        out << conditions[i].description;
    }
    return out.str();
}

PushRuleSet parsePushRules(const std::string& apiResponseJson) {
    PushRuleSet set;

    // Parse each rule kind: "override", "content", "room", "sender", "underride"
    static const char* kinds[] = {"override", "content", "room", "sender", "underride"};

    for (const auto* kind : kinds) {
        std::string searchKey = '"' + std::string(kind) + '"';
        size_t pos = 0;
        while (true) {
            pos = apiResponseJson.find(searchKey, pos);
            if (pos == std::string::npos) break;

            auto objStart = apiResponseJson.rfind('{', pos);
            if (objStart == std::string::npos) break;

            int depth = 0;
            auto objEnd = objStart;
            while (objEnd < apiResponseJson.size()) {
                if (apiResponseJson[objEnd] == '{') ++depth;
                else if (apiResponseJson[objEnd] == '}') --depth;
                if (depth == 0) break;
                ++objEnd;
            }
            if (objEnd >= apiResponseJson.size()) break;

            std::string obj = apiResponseJson.substr(objStart, objEnd - objStart + 1);

            PushRule rule;
            rule.ruleId  = parseJsonStringValue(obj, "rule_id");
            rule.kind    = std::string(kind);

            auto enabled = parseJsonStringValue(obj, "enabled");
            rule.enabled = (enabled != "false");

            // Parse conditions
            auto condJson = parseJsonStringValue(obj, "conditions");
            if (!condJson.empty()) {
                // Parse condition array manually
                size_t cpos = 0;
                while (true) {
                    cpos = condJson.find("\"kind\"", cpos);
                    if (cpos == std::string::npos) break;

                    auto cObjStart = condJson.rfind('{', cpos);
                    if (cObjStart == std::string::npos) break;

                    int cdepth = 0;
                    auto cObjEnd = cObjStart;
                    while (cObjEnd < condJson.size()) {
                        if (condJson[cObjEnd] == '{') ++cdepth;
                        else if (condJson[cObjEnd] == '}') --cdepth;
                        if (cdepth == 0) break;
                        ++cObjEnd;
                    }
                    if (cObjEnd >= condJson.size()) break;

                    std::string cObj = condJson.substr(cObjStart, cObjEnd - cObjStart + 1);
                    rule.conditions.push_back(parsePushCondition(cObj));
                    cpos = cObjEnd + 1;
                }
            }

            // Parse actions
            auto actionsStr = parseJsonStringValue(obj, "actions");
            if (!actionsStr.empty()) {
                // Simple comma-separated action list
                std::istringstream stream(actionsStr);
                std::string action;
                while (std::getline(stream, action, ',')) {
                    while (!action.empty() && action.front() == ' ') action.erase(0, 1);
                    while (!action.empty() && action.back() == ' ') action.pop_back();
                    rule.actions.push_back(action);
                }
            }

            set.rules.push_back(rule);
            set.totalRules++;
            if (rule.enabled) set.enabledRules++;

            pos = objEnd + 1;
        }
    }

    return set;
}

std::string formatPushRuleItem(const PushRule& rule) {
    std::ostringstream out;
    if (!rule.enabled) out << "[Disabled] ";
    out << rule.ruleId;
    if (!rule.conditions.empty()) {
        out << "\n" << formatPushRuleConditions(rule.conditions);
    }
    return out.str();
}

bool isKnownPushRuleKind(const std::string& kind) {
    return kind == "override" || kind == "underride" || kind == "content" ||
           kind == "room" || kind == "sender";
}

std::string getRuleKindDescription(const std::string& kind, bool enabled) {
    if (kind == "override") return enabled ? "Custom rule" : "Disabled custom rule";
    if (kind == "content")  return "Content match rule";
    if (kind == "room")     return "Per-room rule";
    if (kind == "sender")   return "Per-sender rule";
    if (kind == "underride") return "Default fallback rule";
    return kind;
}

// ---- MSC3061 ----

bool isMsc3061SharedKey(const std::string& roomKeyContentJson) {
    return roomKeyContentJson.find("org.matrix.msc3061.shared_history") != std::string::npos;
}

std::string formatMsc3061Status(bool isShared, const std::string& visibilitySetting) {
    if (isShared) {
        return "Room history sharing is enabled (MSC3061). Visibility: " + visibilitySetting;
    }
    return "Room history is not shared.";
}

bool canShareHistory(const std::string& roomVisibility) {
    return roomVisibility == "world_readable" || roomVisibility == "shared";
}

} // namespace progressive
