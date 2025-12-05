#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "npc.hpp"
#include "observer.hpp"
#include <vector>
#include <memory>

class BattleVisitor {
private:
    double battleRange;     
    Observer* observer;      

public:
    BattleVisitor(double range, Observer* obs);
    
    // Запустить бой между всеми NPC
    void startBattle(std::vector<std::shared_ptr<NPC>>& npcs);
    
    // Геттер для дистанции
    double getBattleRange() const { return battleRange; }
    
    // Сеттер для дистанции
    void setBattleRange(double range) { battleRange = range; }
};

#endif