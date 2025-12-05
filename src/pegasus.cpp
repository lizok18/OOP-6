#include "pegasus.hpp"
#include <iostream>

Pegasus::Pegasus(double x, double y, const std::string& name)
    : NPC(PEGASUS, x, y, name) {}

void Pegasus::print() const {
    std::cout << "Пегас " << name << " (" << x << ", " << y << ")" 
              << (alive ? " - жив" : " - мертв") << std::endl;
}

bool Pegasus::fight(const NPC* other) {
    switch(other->getType()) {
        case KNIGHT:
            // Пегас vs Рыцарь - преимущество
            return (rand() % 100) < 60;
        case DRAGON:
            // Пегас vs Дракон - слабость
            return (rand() % 100) < 20;
        case PEGASUS:
            // Пегас vs Пегас - шанс 50/50
            return (rand() % 100) < 50;
        default:
            return false;
    }
}