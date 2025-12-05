#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "npc.hpp"
#include "knight.hpp"
#include "dragon.hpp"
#include "pegasus.hpp"
#include <memory>
#include <vector>

class Factory {
public:
    // Создать NPC
    static std::shared_ptr<NPC> create(NPCType type, const std::string& name, double x, double y);
    
    // Загрузить NPC из потока
    static std::shared_ptr<NPC> load(std::istream& in);
    
    // Сохранить всех NPC в файл
    static void saveToFile(const std::vector<std::shared_ptr<NPC>>& npcs, const std::string& filename);
    
    // Загрузить NPC из файла
    static std::vector<std::shared_ptr<NPC>> loadFromFile(const std::string& filename);
};

#endif