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