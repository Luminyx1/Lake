#pragma once

#include "Lake/Common.h"

#include <array>
#include <iostream>
#include <ctime>

namespace lake {

    namespace LogColor {
        // Formatting
        static const char* Reset        = "\33[0m";
        static const char* Bold         = "\33[1m";
        static const char* NoBold       = "\33[22m";
        static const char* Underline    = "\33[4m";
        static const char* NoUnderline  = "\33[24m";
        static const char* Invert       = "\33[7m";
        static const char* NoInvert     = "\33[27m";

        // Colors
        static const char* Black        = "\33[30m";
        static const char* Red          = "\33[31m";
        static const char* Green        = "\33[32m";
        static const char* Yellow       = "\33[33m";
        static const char* Blue         = "\33[34m";
        static const char* Magenta      = "\33[35m";
        static const char* Cyan         = "\33[36m";
        static const char* LightGray    = "\33[37m";
        static const char* Gray         = "\33[90m";
        static const char* LightRed     = "\33[91m";
        static const char* LightGreen   = "\33[92m";
        static const char* LightYellow  = "\33[93m";
        static const char* LightBlue    = "\33[94m";
        static const char* LightMagenta = "\33[95m";
        static const char* LightCyan    = "\33[96m";
        static const char* White        = "\33[97m";
    } // namespace LogColor

    namespace internal {
        template <typename T>
        void print(const T& t) {
            std::cout << t;
        }

        template <typename T, typename... Args>
        void print(const T& t, const Args&... args) {
            std::cout << t;
            print(args...);
        }

        inline void printTime() {
            std::array<char, 9> buffer = { 0 };
            const std::time_t time = std::time(nullptr);
            std::strftime(buffer.data(), buffer.size(), "%H:%M:%S", std::localtime(&time));
            std::cout << "[" << buffer.data() << "] ";
        }

        template <typename... Args>
        void logEntry(const char* color, const char* type, Args... args) {
            printTime();
            std::cout << color <<
            #ifdef LK_INTERNAL
                "[lake/"
            #else
                "[app/"
            #endif
            << type << "]: ";
            print(args...);
            std::cout << LogColor::Reset << std::endl;
        }
    } // namespace internal

    template <typename... Args>
    void trace(Args... args) {
        #ifndef LK_DISABLE_LOG_TRACE
            internal::logEntry(LogColor::Gray, "TRACE", args...);
        #endif
    }

    template <typename... Args>
    void info(Args... args) {
        #ifndef LK_DISABLE_LOG_INFO
            internal::logEntry(LogColor::White, "INFO", args...);
        #endif
    }

    template <typename... Args>
    void warn(Args... args) {
        #ifndef LK_DISABLE_LOG_WARN
            internal::logEntry(LogColor::Yellow, "WARN", args...);
        #endif
    }

    template <typename... Args>
    void error(Args... args) {
        #ifndef LK_DISABLE_LOG_ERROR
            internal::logEntry(LogColor::Red, "ERROR", args...);
        #endif
    }

} // namespace lake

#ifndef LK_DIST
    #define LK_ASSERT(x, ...) do { if (!(x)) [[unlikely]] { lake::error(LK_FILENAME, "(", __LINE__, "): Assert failed! ", __VA_ARGS__ ); LK_BREAKPOINT(); } } while (false)
#else
    #define LK_ASSERT(x, ...) do { (void)(x); } while (false)
#endif // LK_DIST
