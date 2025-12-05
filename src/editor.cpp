#include "editor.hpp"
#include "factory.hpp"
#include "visitor.hpp"
#include <algorithm>
#include <iostream>

// Конструктор редактора
DungeonEditor::DungeonEditor(Observer* obs) : observer(obs) {
    if (observer) {
        observer->update("Редактор подземелья инициализирован");
    }
}

// Добавить NPC
void DungeonEditor::addNPC(NPCType type, const std::string& name, double x, double y) {
    // Проверка координат
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        if (observer) observer->update("Ошибка: координаты вне диапазона 0-500");
        return;
    }
    
    // Проверка уникальности имени
    for (const auto& npc : npcs) {
        if (npc->getName() == name) {
            if (observer) observer->update("Ошибка: имя '" + name + "' уже занято");
            return;
        }
    }
    
    // Создание NPC через фабрику
    auto npc = Factory::create(type, name, x, y);
    if (npc) {
        npcs.push_back(npc);
        if (observer) {
            observer->update("Добавлен NPC: " + name + 
                           " (" + std::to_string(x) + ", " + std::to_string(y) + ")");
        }
    } else {
        if (observer) observer->update("Ошибка: не удалось создать NPC");
    }
}

// Показать всех NPC
void DungeonEditor::showAll() const {
    if (npcs.empty()) {
        std::cout << "Нет NPC в подземелье." << std::endl;
        return;
    }
    
    std::cout << "\n=== СПИСОК NPC (" << npcs.size() << ") ===" << std::endl;
    for (size_t i = 0; i < npcs.size(); i++) {
        std::cout << i + 1 << ". ";
        npcs[i]->print();
    }
}

void DungeonEditor::save(const std::string& filename) {
    Factory::saveToFile(npcs, filename);
    if (observer) {
        observer->update("Сохранено " + std::to_string(npcs.size()) + 
                        " NPC в файл: " + filename);
    }
}

void DungeonEditor::load(const std::string& filename) {
    auto loaded = Factory::loadFromFile(filename);
    npcs.insert(npcs.end(), loaded.begin(), loaded.end());
    
    if (observer) {
        observer->update("Загружено " + std::to_string(loaded.size()) + 
                        " NPC из файла: " + filename);
    }
}

void DungeonEditor::startBattle(double range) {
    if (npcs.size() < 2) {
        if (observer) {
            observer->update("Ошибка: для боя нужно минимум 2 NPC, сейчас " + 
                           std::to_string(npcs.size()));
        }
        return;
    }
    
    if (observer) {
        observer->update("Начинается бой на дистанции " + std::to_string(range) +
                        " метров с " + std::to_string(npcs.size()) + " участниками");
    }
    
    BattleVisitor battle(range, observer);
    battle.startBattle(npcs);
}