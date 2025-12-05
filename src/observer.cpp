#include "observer.hpp"
#include <iostream>

ConsoleObserver::ConsoleObserver() {
}

void ConsoleObserver::update(const std::string& message) {
    std::cout << "[СОБЫТИЕ] " << message << std::endl;
}

FileObserver::FileObserver(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    
    if (!logFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
    } else {
        logFile << "=== ЛОГ СИМУЛЯЦИИ ===" << std::endl;
    }
}

FileObserver::~FileObserver() {
    if (logFile.is_open()) {
        logFile << "=== КОНЕЦ ЛОГА ===" << std::endl;
        logFile.close();
    }
}

void FileObserver::update(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
}

bool FileObserver::isFileOpen() const {
    return logFile.is_open();
}