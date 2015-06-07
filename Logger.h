#pragma once

#include <iostream>
#include <mutex>
#include <sstream>

// Implicit conversion to int is intended
enum LogLevels {
    ERROR   = 1,
    WARNING = 2,
    INFO    = 3,
    DEBUG   = 4,
    MISC    = 5, // and less
};

inline const char * logLevelToString(unsigned level) {
    switch(level) {
    case ERROR:
        return "ERRO";
    case WARNING:
        return "WARN";
    case INFO:
        return "INFO";
    case DEBUG:
        return "DEBG";
    default:
        return "MISC";
    }
}

// Shamelessly adapted from chromium
// Use LOG macro, which is defined at the bottom
class Logger {
public:
    static inline void setLogLevel(unsigned level) noexcept { s_currentLogLevel = level; }
    static inline unsigned logLevel() noexcept { return s_currentLogLevel; }

    Logger(const char *file, int line, unsigned level)
        : m_level(level)
    {
        m_buffer << file << ":" << line << " " << logLevelToString(m_level) << ": ";
    }

    ~Logger()
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        if (m_level <= logLevel()) {
            if (m_level <= WARNING) {
                std::clog << m_buffer.rdbuf() << std::endl;
            } else {
                std::cout << m_buffer.rdbuf() << std::endl;
            }
        }
    }

    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger & operator=(const Logger &) = delete;
    Logger & operator=(Logger &&) = delete;

    std::ostream& stream() { return m_buffer; }

private:
    static unsigned s_currentLogLevel;
    static std::mutex s_mutex;

    std::stringstream m_buffer;
    const unsigned m_level;
};

#define LOG(level) Logger(__FILE__, __LINE__, level).stream()

