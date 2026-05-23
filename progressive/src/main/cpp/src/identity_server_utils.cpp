#include "progressive/identity_server_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildIdServerBindRequest(const std::string& sid, const std::string& secret, const std::string& mxid){std::ostringstream os;os<<R"({"sid":")"<<sid<<R"(","client_secret":")"<<secret<<R"(","mxid":")"<<mxid<<R"("})";return os.str();}
std::string buildIdServerLookupRequest(const std::string& addr, const std::string& medium){std::ostringstream os;os<<R"({"address":")"<<addr<<R"(","medium":")"<<medium<<R"("})";return os.str();}
}
