//
// Created by dustyn on 6/27/26.
//

#ifndef LIBREVAULT_LOG_H
#define LIBREVAULT_LOG_H
#include <iostream>
#include <ostream>
#include <string>


enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    CRITICAL = 4
};

#define DEBUG LogLevel::DEBUG
#define INFO LogLevel::INFO
#define WARN LogLevel::WARN
#define ERROR LogLevel::ERROR
#define CRITICAL LogLevel::CRITICAL


#define LOGLEVEL DEBUG

struct Logger {
    LogLevel level;

    Logger() {
        this->level = WARN;
    }

    explicit Logger(LogLevel level) {
        this->level = level;
    }

    void log(const LogLevel log_level, const std::string &function, const std::string &message) const {
        if (log_level < this->level) {
            return;
        }
        switch (log_level) {
            case DEBUG:
                std::cout << "[DEBUG] in function: " << function << " -> " << message << std::endl;
                break;
            case INFO:
                std::cout << "[INFO] in function: " << function << " -> " << message << std::endl;
                break;
            case WARN:
                std::cout << "[WARN] in function: " << function << " -> " << message << std::endl;
                break;
            case ERROR:
                std::cerr << "[ERROR] in function: " << function << " -> " << message << std::endl;
                break;
            case CRITICAL:
                std::cerr << "[CRITICAL] in function: " << function << " -> " << message << std::endl;
                break;
        }
    }
};

static inline auto logger = Logger(LOGLEVEL);
//we will eventually parse this from a config file or just command line but for now we'll use a constant one

#endif //LIBREVAULT_LOG_H
