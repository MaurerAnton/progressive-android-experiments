#include "progressive/three_pid_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildEmailRequest(const std::string& email, const std::string& secret, int attempt) { std::ostringstream os; os<<R"({"client_secret":")"<<secret<<R"(","email":")"<<email<<R"(","send_attempt":)"<<attempt<<"}"; return os.str(); }
std::string buildPhoneRequest(const std::string& phone, const std::string& secret, int attempt) { std::ostringstream os; os<<R"({"client_secret":")"<<secret<<R"(","phone_number":")"<<phone<<R"(","send_attempt":)"<<attempt<<"}"; return os.str(); }
ThreePid parseThreePid(const std::string& json) { ThreePid t; auto e=[&](const std::string& k)->std::string{auto p=json.find("\""+k+"\":\"");if(p==std::string::npos)return"";p+=k.size()+4;auto ee=json.find('"',p);return ee!=std::string::npos?json.substr(p,ee-p):"";}; t.medium=e("medium");t.address=e("address"); return t; }
bool isEmailBound(const std::string& json, const std::string& email) { return json.find("\""+email+"\"")!=std::string::npos; }
}
