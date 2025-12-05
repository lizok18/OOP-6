#ifndef PEGASUS_HPP
#define PEGASUS_HPP

#include "npc.hpp"

class Pegasus : public NPC {
public:
    Pegasus(double x, double y, const std::string& name);
    void print() const override;
    bool fight(const NPC* other) override;
};

#endif