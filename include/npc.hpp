#ifndef NPC_HPP
#define NPC_HPP

#include <string>
#include <memory>

// Типы NPC
enum NPCType {
    KNIGHT = 1,
    DRAGON = 2,
    PEGASUS = 3
};

// Базовый класс NPC
class NPC {
protected:
    NPCType type;
    double x, y;
    std::string name;
    bool alive;

public:
    NPC(NPCType t, double x, double y, const std::string& name);
    virtual ~NPC() = default;
    
    // Геттеры
    NPCType getType() const { return type; }
    double getX() const { return x; }
    double getY() const { return y; }
    std::string getName() const { return name; }
    bool isAlive() const { return alive; }
    
    // Методы
    void kill() { alive = false; }
    double distanceTo(const NPC* other) const;
    bool isClose(const NPC* other, double range) const;
    
    virtual void print() const = 0;
    virtual bool fight(const NPC* other) = 0;
};

#endif