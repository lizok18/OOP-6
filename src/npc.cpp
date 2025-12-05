#include "npc.hpp"
#include <cmath>
#include <iostream>

NPC::NPC(NPCType t, double x, double y, const std::string& name)
    : type(t), x(x), y(y), name(name), alive(true) {}

double NPC::distanceTo(const NPC* other) const {
    return sqrt(pow(x - other->x, 2) + pow(y - other->y, 2));
}

bool NPC::isClose(const NPC* other, double range) const {
    return distanceTo(other) <= range;
}