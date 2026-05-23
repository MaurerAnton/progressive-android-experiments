#pragma once
#include <string>

std::string formatHtmlToPlain(const std::string& json);
std::string formatPlainToHtml(const std::string& json);
std::string sanitizeHtml(const std::string& json);
