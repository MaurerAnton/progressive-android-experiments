#include "progressive/url_render_utils.hpp"
#include <sstream>
namespace progressive {
std::string buildUrlPreviewHtml(const std::string& url, const std::string& title, const std::string& desc, const std::string& img, const std::string& site){std::ostringstream os;os<<"<div class=\"url-preview\">";if(!img.empty())os<<"<img src=\""<<img<<"\"/>";os<<"<strong>"<<title<<"</strong><br/>"<<desc<<"<br/><small>"<<site<<"</small></div>";return os.str();}
std::string formatUrlForDisplay(const std::string& url, int max){if((int)url.size()<=max)return url;return url.substr(0,max-3)+"...";}
}
