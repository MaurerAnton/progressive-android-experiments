#pragma once
#include <string>

std::string verifyEventSignature(const std::string& json);
std::string signEvent(const std::string& json);
std::string parseSignatures(const std::string& json);
