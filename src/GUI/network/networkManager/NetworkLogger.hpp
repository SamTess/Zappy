#pragma once
#include <iostream>
#include <mutex>
#include <string>

class NetworkLogger {
public:
    static NetworkLogger& get() {
        static NetworkLogger instance;
        return instance;
    }
    void setEnabled(bool enabled) {
        _enabled = enabled;
    }
    void log(const std::string& msg) {
        if (_enabled) {
            std::lock_guard<std::mutex> lock(_mutex);
            std::cout << "[DEBUG] " << msg << std::endl;
        }
    }
    bool isEnabled() const { return _enabled; }
private:
    NetworkLogger() : _enabled(false) {}
    bool _enabled;
    std::mutex _mutex;
};
