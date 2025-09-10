#pragma once

#include <string>
#include <format>
#include <algorithm>
#include <array>
#include <iostream>
#include <filesystem>

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

        std::cout << m_Options.WarningPrefix + text << "\n";
    }

    template<typename ...Args>
    static void Error(std::string_view fmt, Args&&... args)
    {
        std::string text = std::vformat(fmt, std::make_format_args(args...));
        AddColor(text);

        std::cout << m_Options.ErrorPrefix + text << "\n";
    }


private:
    static void AddColor(std::string& text);
};