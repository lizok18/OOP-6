#include "factory.hpp"
#include <fstream>
#include <sstream>

std::shared_ptr<NPC> Factory::create(NPCType type, const std::string& name, double x, double y) {
    switch(type) {
        case KNIGHT:
            return std::make_shared<Knight>(x, y, name);
        case DRAGON:
            return std::make_shared<Dragon>(x, y, name);
        case PEGASUS:
            return std::make_shared<Pegasus>(x, y, name);
        default:
            return nullptr;
    }
}

std::shared_ptr<NPC> Factory::load(std::istream& in) {
    int typeInt;
    std::string name;
    double x, y;
    
    if (in >> typeInt >> name >> x >> y) {
        return create(static_cast<NPCType>(typeInt), name, x, y);
    }
    return nullptr;
}

void Factory::saveToFile(const std::vector<std::shared_ptr<NPC>>& npcs, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& npc : npcs) {
        file << static_cast<int>(npc->getType()) << " "
             << npc->getName() << " "
             << npc->getX() << " "
             << npc->getY() << std::endl;
    }
}

std::vector<std::shared_ptr<NPC>> Factory::loadFromFile(const std::string& filename) {
    std::vector<std::shared_ptr<NPC>> npcs;
    std::ifstream file(filename);
    
    if (!file) return npcs;
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        if (auto npc = load(iss)) {
            npcs.push_back(npc);
        }
    }
    
    return npcs;
}