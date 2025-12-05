#include "dragon.hpp"
#include <iostream>

Dragon::Dragon(double x, double y, const std::string& name)
    : NPC(DRAGON, x, y, name) {}

void Dragon::print() const {
    std::cout << "Дракон " << name << " (" << x << ", " << y << ")" 
              << (alive ? " - жив" : " - мертв") << std::endl;
}

bool Dragon::fight(const NPC* other) {
    switch(other->getType()) {
        case KNIGHT:
            // Дракон vs Рыцарь - слабость
            return (rand() % 100) < 30;
        case DRAGON:
            // Дракон vs Дракон - шанс 50/50
            return (rand() % 100) < 50;
        case PEGASUS:
            // Дракон vs Пегас - преимущество
            return (rand() % 100) < 80;
        default:
            return false;
    }
}