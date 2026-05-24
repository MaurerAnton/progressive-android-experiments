#include "progressive/wellknown_helper.hpp"
#include <sstream>

namespace progressive {

WellKnownResult parseWellKnown(const std::string& json) {
    WellKnownResult r;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    
    r.baseUrl = extract("base_url");
    r.identityServer = extract("identity_server");
    r.isValid = !r.baseUrl.empty() || !r.identityServer.empty();
    
    if (json.find("\"m.homeserver\":") != std::string::npos) {
        auto hsStart = json.find("\"base_url\":\"", json.find("\"m.homeserver\""));
        if (hsStart != std::string::npos) {
            hsStart += 12;
            auto hsEnd = json.find('"', hsStart);
            if (hsEnd != std::string::npos) r.baseUrl = json.substr(hsStart, hsEnd - hsStart);
        }
        r.isValid = !r.baseUrl.empty();
    }
    
    return r;
}

std::string buildWellKnownUrl(const std::string& domain) {
    return "https://" + domain + "/.well-known/matrix/client";
}

bool supportsFeature(const std::string& json, const std::string& feature) {
    return json.find(feature) != std::string::npos;
}

std::string extractServerFromMxid(const std::string& mxid) {
    auto colon = mxid.find(':');
    if (colon == std::string::npos) return mxid;
    return mxid.substr(colon + 1);
}

std::string buildLoginUrl(const WellKnownResult& wellKnown) {
    if (!wellKnown.isValid) return "";
    std::string base = wellKnown.baseUrl;
    while (!base.empty() && base.back() == '/') base.pop_back();
    return base + "/_matrix/client/v3/login";
}

} // namespace progressive
