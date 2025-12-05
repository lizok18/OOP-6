#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "npc.hpp"

class Knight : public NPC {
public:
    Knight(double x, double y, const std::string& name);
    void print() const override;
    bool fight(const NPC* other) override;
};

#endif