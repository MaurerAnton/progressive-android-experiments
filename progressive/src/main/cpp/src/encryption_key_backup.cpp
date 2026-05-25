#include "progressive/encryption_key_backup.hpp"
#include <sstream>

std::string backupKeys(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"backupKeys"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string restoreKeys(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"restoreKeys"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getBackupVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getBackupVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string deleteBackup(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"deleteBackup"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
