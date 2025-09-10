#include "logger.h"

std::array<std::string, COLORS_COUNT> Logger::TextColorsKeys =
{
    "d", // Default
    "r", // Red
    "g", // Green
    "y", // Yellow
    "b", // Blue
    "p", // Purple / Magenta
    "c", // Cyan
    "w"  // White
};

std::array<std::string, COLORS_COUNT> Logger::TextColors =
{
    "\033[0m",     // Default
    "\033[0;31m",  // Red
    "\033[0;32m",  // Green
    "\033[0;33m",  // Yellow
    "\033[0;34m",  // Blue
    "\033[0;35m",  // Purple / Magenta
    "\033[0;36m",  // Cyan
    "\033[0;37m",  // White
};

Logger::Options Logger::m_Options = {};

void Logger::SetOptions(const Logger::Options& options)
{
    m_Options = options;

    AddColor(m_Options.InfoPrefix);
    AddColor(m_Options.WarningPrefix);
    AddColor(m_Options.ErrorPrefix);
}

void Logger::AddColor(std::string& text)
{
    size_t pos = 0;

    while ((pos = text.find(m_Options.ColorPrefix, pos)) != std::string::npos)
    {
        if (pos + 1 >= text.size()) break;

        std::string key(1, text[pos + 1]);
        auto colorIndex = std::find(TextColorsKeys.begin(), TextColorsKeys.end(), key);

        if (colorIndex != TextColorsKeys.end())
        {
            size_t colorCodeIndex = std::distance(TextColorsKeys.begin(), colorIndex);
            const std::string& ansiCode = TextColors[colorCodeIndex];

            text.replace(pos, 2, ansiCode);
            pos += ansiCode.size();
        }
        else
        {
            ++pos;
        }
    }

    text.append(TextColors[(size_t)TextColor::Default]);
}