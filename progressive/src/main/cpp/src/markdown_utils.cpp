#include "progressive/markdown_utils.hpp"
#include <sstream>

std::string parseMarkdownToHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseMarkdownToHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseHtmlToPlain(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseHtmlToPlain"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string stripMarkdownSyntax(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"stripMarkdownSyntax"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractLinksFromMarkdown(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractLinksFromMarkdown"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
