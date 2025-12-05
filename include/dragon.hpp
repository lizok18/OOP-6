#ifndef DRAGON_HPP
#define DRAGON_HPP

#include "npc.hpp"

class Dragon : public NPC {
public:
    Dragon(double x, double y, const std::string& name);
    void print() const override;
    bool fight(const NPC* other) override;
};

#endif
