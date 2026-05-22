#pragma once
#include <string>

namespace progressive {

// Convert text to rainbow-colored HTML
std::string textToRainbow(const std::string& text, double saturation = 1.0, double lightness = 0.5);

// Convert text to wave-colored HTML  
std::string textToWave(const std::string& text);

// Get HTML color for character position in rainbow
std::string getRainbowColor(int index, int totalChars);

// Check if text should be rainbow-ified (starts with /rainbow or !rainbow)
bool isRainbowCommand(const std::string& text);

// Remove rainbow/fancy formatting commands from text
std::string stripFormatCommands(const std::string& text);

} // namespace progressive
