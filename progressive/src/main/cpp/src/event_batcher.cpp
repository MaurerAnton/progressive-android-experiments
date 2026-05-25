#include "progressive/event_batcher.hpp"
#include <sstream>

std::string addToBatch(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"addToBatch"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string flushBatch(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"flushBatch"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string batchSize(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"batchSize"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getPendingEvents(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getPendingEvents"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
