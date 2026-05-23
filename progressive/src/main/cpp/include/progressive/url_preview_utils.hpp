#pragma once
#include <string>

std::string parseOpenGraph(const std::string& json);
std::string extractMetadata(const std::string& json);
std::string buildPreviewSnippet(const std::string& json);
