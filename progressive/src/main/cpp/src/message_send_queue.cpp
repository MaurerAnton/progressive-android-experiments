#include "progressive/message_send_queue.hpp"
#include <sstream>

std::string enqueueSend(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"enqueueSend"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string dequeueSend(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"dequeueSend"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getQueueSize(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getQueueSize"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string cancelSend(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"cancelSend"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
