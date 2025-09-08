#pragma once

#include <string>
#include <format>
#include <algorithm>
#include <array>
#include <iostream>

constexpr int COLORS_COUNT = 8;

class Logger
{
public:
    enum class TextColor
    {
        Default,
        Red,
        Green,
        Yellow,
        Blue,
        Purple,
        Cyan,
        White,
    };

    struct Options
    {
        std::string ColorPrefix;
        std::string InfoPrefix;
        std::string WarningPrefix;
        std::string ErrorPrefix;
    };

private:
    static Options m_Options;
    static std::array<std::string, COLORS_COUNT> TextColorsKeys;
    static std::array<std::string, COLORS_COUNT> TextColors;

public:
    static void SetOptions(const Options& options);

	template<typename ...Args>
    static void Info(std::string_view fmt, Args&&... args)
    {
        std::string text = std::vformat(fmt, std::make_format_args(args...));
        AddColor(text);

        std::cout << m_Options.InfoPrefix + text << "\n";
    }

    template<typename ...Args>
    static void Warning(std::string_view fmt, Args&&... args)
    {
        std::string text = std::vformat(fmt, std::make_format_args(args...));
        AddColor(text);
       
        std::cout << m_Options.WarningPrefix + text  << "\n";
    }

    template<typename ...Args>
    static void Error(std::string_view fmt, Args&&... args)
    {
        std::string text = std::vformat(fmt, std::make_format_args(args...));
        AddColor(text);

        std::cout << m_Options.ErrorPrefix + text << "\n";
    }

private:
    static void AddColor(std::string& text)
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
};