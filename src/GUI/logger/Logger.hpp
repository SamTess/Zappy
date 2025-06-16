/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Logger
*/

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
} LogLevel;

class Logger {
    public:
        explicit Logger(std::string logFilePath, bool isLoggingEnabled = true);
        ~Logger();
        void setLoggingEnabled(bool isEnabled);
        bool isLoggingEnabled() const;
        void logInfo(const std::string &message);
        void logWarning(const std::string &message);
        void logError(const std::string &message);
        void logDebug(const std::string &message);
        std::string getLogFilePath() const;
        void setLogFilePath(const std::string &logFilePath);

    protected:
    private:
        std::string _logFilePath;
        bool _isLoggingEnabled;
        void logMessage(LogLevel level, const std::string &message);
        void logToFile(const std::string &message);
};

#endif /* !LOGGER_HPP_ */
