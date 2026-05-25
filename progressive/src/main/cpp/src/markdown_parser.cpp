#include "progressive/markdown_parser.hpp"
#include <sstream>

std::string parseMarkdown(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseMarkdown"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractHeadings(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractHeadings"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractLinks(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractLinks"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractCodeBlocks(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractCodeBlocks"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
