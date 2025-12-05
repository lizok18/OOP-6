#include "visitor.hpp"
#include "npc.hpp"
#include <algorithm>
#include <iostream>

BattleVisitor::BattleVisitor(double range, Observer* obs) 
    : battleRange(range), observer(obs) {}

// Метод для запуска боя
void BattleVisitor::startBattle(std::vector<std::shared_ptr<NPC>>& npcs) {
    observer->update("=== НАЧАЛО БОЯ ===");
    
    // Проходим по всем парам NPC
    for (size_t i = 0; i < npcs.size(); i++) {
        // Проверяем, жив ли первый NPC
        if (!npcs[i]->isAlive()) continue;
        
        for (size_t j = i + 1; j < npcs.size(); j++) {
            // Проверяем, жив ли второй NPC
            if (!npcs[j]->isAlive()) continue;
            
            if (npcs[i]->isClose(npcs[j].get(), battleRange)) {

                bool result = npcs[i]->fight(npcs[j].get());
                
                // Логируем результат боя
                if (result) {
                    npcs[j]->kill(); // Второй NPC погибает
                    observer->update(npcs[i]->getName() + " победил " + npcs[j]->getName());
                } else {
                    npcs[i]->kill(); // Первый NPC погибает
                    observer->update(npcs[j]->getName() + " победил " + npcs[i]->getName());
                }
            }
        }
    }
    
    npcs.erase(std::remove_if(npcs.begin(), npcs.end(),
        [](const std::shared_ptr<NPC>& npc) { 
            return !npc->isAlive(); 
        }), 
        npcs.end());
    
    observer->update("=== БОЙ ЗАВЕРШЕН ===");
    observer->update("Осталось в живых: " + std::to_string(npcs.size()));
}