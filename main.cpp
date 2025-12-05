#include <iostream>
#include "editor.hpp"
#include "observer.hpp"

int main() {
    std::cout << "=== РЕДАКТОР ПОДЗЕМЕЛЬЯ ===" << std::endl;
    std::cout << "Типы NPC: 1-Рыцарь, 2-Дракон, 3-Пегас" << std::endl;
    
    // Создаем двух наблюдателей
    ConsoleObserver consoleObserver;
    FileObserver fileObserver("battle_log.txt");
    
    // Используем консольного наблюдателя для редактора
    DungeonEditor editor(&consoleObserver);
    
    int choice;
    do {
        std::cout << "\n1. Добавить NPC" << std::endl;
        std::cout << "2. Показать всех NPC" << std::endl;
        std::cout << "3. Сохранить в файл" << std::endl;
        std::cout << "4. Загрузить из файла" << std::endl;
        std::cout << "5. Запустить бой" << std::endl;
        std::cout << "0. Выйти" << std::endl;
        std::cout << "Выберите действие: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                std::cout << "Тип (1-3): ";
                int type;
                std::cin >> type;
                
                std::cout << "Имя: ";
                std::string name;
                std::cin >> name;
                
                std::cout << "Координата X (0-500): ";
                double x;
                std::cin >> x;
                
                std::cout << "Координата Y (0-500): ";
                double y;
                std::cin >> y;
                
                editor.addNPC(static_cast<NPCType>(type), name, x, y);
                break;
            }
            case 2:
                editor.showAll();
                break;
            case 3:
                editor.save("dungeon.txt");
                break;
            case 4:
                editor.load("dungeon.txt");
                break;
            case 5: {
                std::cout << "Дальность боя (метров): ";
                double range;
                std::cin >> range;
                editor.startBattle(range);
                break;
            }
            case 0:
                std::cout << "Выход из программы..." << std::endl;
                break;
            default:
                std::cout << "Неверный выбор!" << std::endl;
        }
    } while(choice != 0);
    
    return 0;
}