#include "progressive/whois_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildWhoisRequest(const std::string& uid) { return R"({"user_id":")"+uid+R"("})"; }
WhoisResult parseWhoisResponse(const std::string& json, const std::string& uid) { WhoisResult w; w.userId=uid; return w; }
std::string formatWhoisResult(const WhoisResult& w) { return w.userId+(w.displayName.empty()?"":" ("+w.displayName+")"); }
}
