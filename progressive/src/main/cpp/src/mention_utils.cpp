#include "progressive/mention_utils.hpp"
#include <sstream>
namespace progressive {
std::vector<MentionMatch> extractMentions(const std::string& text) {
    std::vector<MentionMatch> m; size_t p=0;
    while((p=text.find('@',p))!=std::string::npos){size_t e=p+1;while(e<text.size()&&text[e]!=' '&&text[e]!='\n')e++;m.push_back({text.substr(p+1,e-p-1),(int)p,(int)e});p=e;}
    return m;
}
std::string buildMentionHtml(const std::string& uid, const std::string& name) {
    std::ostringstream os; os<<"<a href=\"https://matrix.to/#/"<<uid<<"\">@"<<(name.empty()?uid:name)<<"</a>";return os.str();
}
bool hasRoomMention(const std::string& text){return text.find("@room")!=std::string::npos||text.find("@everyone")!=std::string::npos;}
}
