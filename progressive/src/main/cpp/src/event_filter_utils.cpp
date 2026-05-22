#include "progressive/event_filter_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildEventFilter(const std::vector<std::string>& types, const std::vector<std::string>& notTypes,
                               const std::vector<std::string>& senders, const std::vector<std::string>& notSenders, int limit) {
    std::ostringstream os; os << R"({"room":{"timeline":{"limit":)" << limit << "}}";
    if (!types.empty()) { os << R"(,"types":[)"; for (size_t i=0;i<types.size();i++){if(i>0)os<<",";os<<R"(")"<<types[i]<<R"(")";} os<<"]"; }
    os << "}"; return os.str();
}
std::string buildRoomFilter(const std::vector<std::string>& rooms, const std::string& eventFilter) {
    std::ostringstream os; os << R"({"rooms":[)";
    for (size_t i=0;i<rooms.size();i++){if(i>0)os<<",";os<<R"(")"<<rooms[i]<<R"(")";}
    os<<"]}"; return os.str();
}
} // namespace progressive
