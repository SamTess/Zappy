/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Logger
*/

#include "Logger.hpp"
#include <memory>
#include <string>
#include <iostream>

Logger::Logger(std::string logFilePath, bool isLoggingEnabled)
    : _logFilePath(logFilePath), _isLoggingEnabled(isLoggingEnabled) {
}

Logger::~Logger() {
}

void Logger::setLoggingEnabled(bool isEnabled) {
    _isLoggingEnabled = isEnabled;
}

bool Logger::isLoggingEnabled() const {
    return _isLoggingEnabled;
}

void Logger::logInfo(const std::string &message) {
    if (_isLoggingEnabled)
        logMessage(LOG_INFO, message);
}

void Logger::logWarning(const std::string &message) {
    if (_isLoggingEnabled) {
        logMessage(LOG_WARNING, message);
    }
}

void Logger::logError(const std::string &message) {
    if (_isLoggingEnabled)
        logMessage(LOG_ERROR, message);
}

void Logger::logDebug(const std::string &message) {
    if (_isLoggingEnabled)
        logMessage(LOG_DEBUG, message);
}

std::string Logger::getLogFilePath() const {
    return _logFilePath;
}

void Logger::setLogFilePath(const std::string &logFilePath) {
    _logFilePath = logFilePath;
}

void Logger::logMessage(LogLevel level, const std::string &message) {
    std::time_t now = std::time(nullptr);
    std::tm timeinfo_obj;
    localtime_r(&now, &timeinfo_obj);
    std::ostringstream timestamp;
    std::string levelStr;

    timestamp << std::put_time(&timeinfo_obj, "%Y-%m-%d %H:%M:%S");
    switch (level) {
        case LOG_INFO:
            levelStr = "[INFO]";
            break;
        case LOG_WARNING:
            levelStr = "[WARNING]";
            break;
        case LOG_ERROR:
            levelStr = "[ERROR]";
            break;
        case LOG_DEBUG:
            levelStr = "[DEBUG]";
            break;
        default:
            levelStr = "[UNKNOWN]";
            break;
    }

    std::ostringstream logEntry;
    logEntry << "[" << timestamp.str() << "] " << levelStr << " " << message;
    std::cout << logEntry.str() << std::endl;
    if (!_logFilePath.empty()) {
        logToFile(logEntry.str());
    }
}

void Logger::logToFile(const std::string &message) {
    std::ofstream logFile(_logFilePath, std::ios::app);
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    } else {
        std::cerr << "Error: Unable to open log file: " << _logFilePath << std::endl;
    }
}
