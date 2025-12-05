#include "knight.hpp"
#include <iostream>

Knight::Knight(double x, double y, const std::string& name)
    : NPC(KNIGHT, x, y, name) {}

void Knight::print() const {
    std::cout << "Рыцарь " << name << " (" << x << ", " << y << ")" 
              << (alive ? " - жив" : " - мертв") << std::endl;
}

bool Knight::fight(const NPC* other) {
    switch(other->getType()) {
        case KNIGHT:
            // Рыцарь vs Рыцарь - шанс 50/50
            return (rand() % 100) < 50;
        case DRAGON:
            // Рыцарь vs Дракон - преимущество
            return (rand() % 100) < 70;
        case PEGASUS:
            // Рыцарь vs Пегас - сложно
            return (rand() % 100) < 40;
        default:
            return false;
    }
}