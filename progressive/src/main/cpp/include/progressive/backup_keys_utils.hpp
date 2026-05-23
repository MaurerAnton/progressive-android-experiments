#pragma once
#include <string>

std::string generateBackupKey(const std::string& json);
std::string verifyBackupKey(const std::string& json);
std::string encryptBackupData(const std::string& json);
