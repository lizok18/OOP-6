#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <string>
#include <fstream>

class Observer {
public:
    virtual ~Observer() = default;
    
    // Метод для получения обновлений
    virtual void update(const std::string& message) = 0;
};

class ConsoleObserver : public Observer {
public:
    ConsoleObserver();
    void update(const std::string& message) override;
};

class FileObserver : public Observer {
private:
    std::ofstream logFile;  // Файл для записи логов
    
public:
    FileObserver(const std::string& filename);
    
    // Деструктор
    ~FileObserver();
    
    void update(const std::string& message) override;
    
    // Дополнительные методы
    bool isFileOpen() const;
};

#endif