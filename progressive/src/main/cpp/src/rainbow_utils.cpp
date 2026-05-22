#include "progressive/rainbow_utils.hpp"
#include <sstream>
#include <cmath>

namespace progressive {

static std::string hslToHex(double h, double s, double l) {
    double c = (1 - fabs(2 * l - 1)) * s;
    double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = l - c / 2;
    double r = 0, g = 0, b = 0;
    
    if (h < 60) { r = c; g = x; }
    else if (h < 120) { r = x; g = c; }
    else if (h < 180) { g = c; b = x; }
    else if (h < 240) { g = x; b = c; }
    else if (h < 300) { r = x; b = c; }
    else { r = c; b = x; }
    
    auto toHex = [](double v) -> int { return (int)((v + m) * 255); };
    
    char buf[8];
    snprintf(buf, sizeof(buf), "#%02X%02X%02X", toHex(r), toHex(g), toHex(b));
    return buf;
}

std::string getRainbowColor(int index, int total) {
    double hue = (360.0 * index) / (total > 0 ? total : 1);
    return hslToHex(hue, 1.0, 0.5);
}

std::string textToRainbow(const std::string& text, double sat, double light) {
    std::ostringstream os;
    int chars = (int)text.size();
    for (int i = 0; i < chars; i++) {
        if (text[i] == ' ' || text[i] == '\n') { os << text[i]; continue; }
        double hue = (360.0 * i) / chars;
        std::string color = hslToHex(hue, sat, light);
        os << "<font color=\"" << color << "\">" << text[i] << "</font>";
    }
    return os.str();
}

std::string textToWave(const std::string& text) {
    return textToRainbow(text, 0.8, 0.6);
}

bool isRainbowCommand(const std::string& text) {
    return text.find("/rainbow") == 0 || text.find("!rainbow") == 0;
}

std::string stripFormatCommands(const std::string& text) {
    if (text.find("/rainbow ") == 0) return text.substr(9);
    if (text.find("!rainbow ") == 0) return text.substr(9);
    return text;
}

} // namespace progressive
