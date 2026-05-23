#include "progressive/notification_sound_utils.hpp"
#include <sstream>
namespace progressive {
std::string getDefaultNotificationSound(){return "content://settings/system/notification_sound";}
std::string getHighlightNotificationSound(){return "content://settings/system/notification_sound";}
std::string buildSoundTweak(const std::string& uri){std::ostringstream os;os<<R"({"set_tweak":"sound","value":")"<<uri<<R"("})";return os.str();}
}
