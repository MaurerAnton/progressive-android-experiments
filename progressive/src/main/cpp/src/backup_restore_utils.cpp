#include "progressive/backup_restore_utils.hpp"
std::string parseBackup(const std::string&) { return R"({"ok":true})"; }
std::string restoreFromBackup(const std::string&) { return R"({"ok":true})"; }
std::string validateBackup(const std::string&) { return R"({"ok":true})"; }
std::string buildRestoreEvent(const std::string&) { return R"({"ok":true})"; }
std::string formatRestoreProgress(const std::string&) { return R"({"ok":true})"; }
