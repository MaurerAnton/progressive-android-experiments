#include "progressive/widget_event_utils.hpp"
#include <sstream>

namespace progressive {

WidgetEvent parseWidgetEvent(const std::string& key, const std::string& json) {
    WidgetEvent w; w.widgetId = key;
    auto extract = [&](const std::string& k) -> std::string {
        auto p = json.find("\"" + k + "\":\""); if (p == std::string::npos) return "";
        p += k.size() + 4; auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    w.type = extract("type"); w.name = extract("name"); w.url = extract("url");
    return w;
}
std::string buildWidgetApiRequest(const std::string& wid, const std::string& action, const std::string& data) {
    std::ostringstream os;
    os << R"({"widget_id":")" << wid << R"(","action":")" << action << R"(")";
    if (!data.empty()) os << R"(,"data":)" << data;
    os << "}"; return os.str();
}
std::string formatWidgetBadge(const WidgetEvent& w) { return w.name + " [" + w.type + "]"; }
bool isWidgetTypeSupported(const std::string& t) { return t == "jitsi" || t == "m.custom"; }
std::string getWidgetIcon(const std::string& type) {
    if (type == "jitsi") return "ic_widget_jitsi"; return "ic_widget_custom";
}
} // namespace progressive
