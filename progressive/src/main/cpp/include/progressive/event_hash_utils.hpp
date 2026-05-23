#pragma once
#include <string>

std::string verifyEventHash(const std::string& json);
std::string hashEvent(const std::string& json);
std::string computeSha256(const std::string& json);
