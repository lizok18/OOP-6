#include <gtest/gtest.h>
#include "npc.hpp"
#include "knight.hpp"
#include "dragon.hpp"
#include "pegasus.hpp"
#include "factory.hpp"
#include "editor.hpp"
#include "visitor.hpp"
#include "observer.hpp"
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

// ===================== Mock Observer для тестов =====================
class MockObserver : public Observer {
public:
    std::vector<std::string> messages;
    
    void update(const std::string& message) override {
        messages.push_back(message);
    }
    
    void clear() { messages.clear(); }
    
    bool contains(const std::string& text) {
        for (const auto& msg : messages) {
            if (msg.find(text) != std::string::npos) return true;
        }
        return false;
    }
    
    int countContains(const std::string& text) {
        int count = 0;
        for (const auto& msg : messages) {
            if (msg.find(text) != std::string::npos) count++;
        }
        return count;
    }
};

// ===================== Тесты NPC =====================
TEST(NPCTest, CreateNPC) {
    Knight knight(100, 200, "Артур");
    EXPECT_EQ(knight.getType(), KNIGHT);
    EXPECT_EQ(knight.getName(), "Артур");
    EXPECT_EQ(knight.getX(), 100);
    EXPECT_EQ(knight.getY(), 200);
    EXPECT_TRUE(knight.isAlive());
    
    Dragon dragon(300, 400, "Смауг");
    EXPECT_EQ(dragon.getType(), DRAGON);
    EXPECT_EQ(dragon.getName(), "Смауг");
    
    Pegasus pegasus(50, 150, "Пегас");
    EXPECT_EQ(pegasus.getType(), PEGASUS);
    EXPECT_EQ(pegasus.getName(), "Пегас");
}

TEST(NPCTest, DistanceCalculation) {
    Knight knight1(0, 0, "K1");
    Knight knight2(3, 4, "K2");
    EXPECT_NEAR(knight1.distanceTo(&knight2), 5.0, 0.001);
}

TEST(NPCTest, IsCloseCheck) {
    Knight knight1(0, 0, "K1");
    Knight knight2(10, 0, "K2");
    EXPECT_TRUE(knight1.isClose(&knight2, 10.1));
    EXPECT_FALSE(knight1.isClose(&knight2, 9.9));
}

TEST(NPCTest, KillNPC) {
    Knight knight(100, 200, "Артур");
    EXPECT_TRUE(knight.isAlive());
    knight.kill();
    EXPECT_FALSE(knight.isAlive());
}

TEST(NPCTest, PrintNPC) {
    Knight knight(100, 200, "Артур");
    testing::internal::CaptureStdout();
    knight.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Артур") != std::string::npos);
    EXPECT_TRUE(output.find("100") != std::string::npos);
}

// ===================== Тесты Factory =====================
TEST(FactoryTest, CreateNPC) {
    auto knight = Factory::create(KNIGHT, "Артур", 100, 200);
    EXPECT_NE(knight, nullptr);
    EXPECT_EQ(knight->getType(), KNIGHT);
    EXPECT_EQ(knight->getName(), "Артур");
    
    auto dragon = Factory::create(DRAGON, "Смауг", 300, 400);
    EXPECT_NE(dragon, nullptr);
    EXPECT_EQ(dragon->getType(), DRAGON);
    
    auto pegasus = Factory::create(PEGASUS, "Пегас", 50, 150);
    EXPECT_NE(pegasus, nullptr);
    EXPECT_EQ(pegasus->getType(), PEGASUS);
    
    auto unknown = Factory::create(static_cast<NPCType>(999), "Unknown", 0, 0);
    EXPECT_EQ(unknown, nullptr);
}

TEST(FactoryTest, LoadFromStream) {
    std::stringstream ss;
    ss << "1 Артур 100 200\n";
    ss << "2 Смауг 300 400\n";
    ss << "3 Пегас 50 150\n";
    
    auto knight = Factory::load(ss);
    EXPECT_NE(knight, nullptr);
    EXPECT_EQ(knight->getType(), KNIGHT);
    
    auto dragon = Factory::load(ss);
    EXPECT_NE(dragon, nullptr);
    EXPECT_EQ(dragon->getType(), DRAGON);
    
    auto pegasus = Factory::load(ss);
    EXPECT_NE(pegasus, nullptr);
    EXPECT_EQ(pegasus->getType(), PEGASUS);
}

TEST(FactoryTest, SaveAndLoadToFile) {
    std::vector<std::shared_ptr<NPC>> npcs;
    npcs.push_back(Factory::create(KNIGHT, "Артур", 100, 200));
    npcs.push_back(Factory::create(DRAGON, "Смауг", 300, 400));
    npcs.push_back(Factory::create(PEGASUS, "Пегас", 50, 150));
    
    const std::string testFile = "test_npcs.txt";
    Factory::saveToFile(npcs, testFile);
    
    auto loaded = Factory::loadFromFile(testFile);
    EXPECT_EQ(loaded.size(), 3);
    
    if (loaded.size() > 0) {
        EXPECT_EQ(loaded[0]->getType(), KNIGHT);
        EXPECT_EQ(loaded[0]->getName(), "Артур");
    }
    
    std::remove(testFile.c_str());
}

// ===================== Тесты Battle =====================
TEST(BattleTest, TwoNPCsFight) {
    MockObserver observer;
    
    auto knight = std::make_shared<Knight>(0, 0, "Рыцарь");
    auto dragon = std::make_shared<Dragon>(5, 0, "Дракон");
    
    std::vector<std::shared_ptr<NPC>> npcs = {knight, dragon};
    BattleVisitor battle(100, &observer);
    battle.startBattle(npcs);
    
    bool oneDead = !knight->isAlive() || !dragon->isAlive();
    EXPECT_TRUE(oneDead);
    EXPECT_FALSE(observer.messages.empty());
}

TEST(BattleTest, TooFarToFight) {
    MockObserver observer;
    
    auto knight = std::make_shared<Knight>(0, 0, "Рыцарь");
    auto dragon = std::make_shared<Dragon>(100, 100, "Дракон");
    
    std::vector<std::shared_ptr<NPC>> npcs = {knight, dragon};
    BattleVisitor battle(50, &observer);
    battle.startBattle(npcs);
    
    EXPECT_TRUE(knight->isAlive());
    EXPECT_TRUE(dragon->isAlive());
    EXPECT_EQ(npcs.size(), 2);
}

TEST(BattleTest, DeadNPCsDontFight) {
    MockObserver observer;
    
    auto knight = std::make_shared<Knight>(0, 0, "Рыцарь");
    auto dragon = std::make_shared<Dragon>(10, 0, "Дракон");
    knight->kill();
    
    std::vector<std::shared_ptr<NPC>> npcs = {knight, dragon};
    BattleVisitor battle(100, &observer);
    battle.startBattle(npcs);
    
    EXPECT_TRUE(dragon->isAlive());
    EXPECT_EQ(npcs.size(), 1);
}

// ===================== Тесты Editor =====================
TEST(EditorTest, AddNPC) {
    MockObserver observer;
    DungeonEditor editor(&observer);
    
    editor.addNPC(KNIGHT, "Артур", 100, 200);
    EXPECT_EQ(editor.getNPCs().size(), 1);
    EXPECT_TRUE(observer.contains("Добавлен"));
    
    editor.addNPC(DRAGON, "Артур", 300, 400);
    EXPECT_EQ(editor.getNPCs().size(), 1);
    EXPECT_TRUE(observer.contains("занято"));
    
    editor.addNPC(DRAGON, "Смауг", 600, 700);
    EXPECT_EQ(editor.getNPCs().size(), 1);
    EXPECT_TRUE(observer.contains("диапазона"));
    
    editor.addNPC(DRAGON, "Смауг", 300, 400);
    EXPECT_EQ(editor.getNPCs().size(), 2);
}

TEST(EditorTest, ShowNPCs) {
    MockObserver observer;
    DungeonEditor editor(&observer);
    
    testing::internal::CaptureStdout();
    editor.showAll();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Нет NPC") != std::string::npos);
    
    editor.addNPC(KNIGHT, "Артур", 100, 200);
    editor.addNPC(DRAGON, "Смауг", 300, 400);
    
    testing::internal::CaptureStdout();
    editor.showAll();
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Артур") != std::string::npos);
    EXPECT_TRUE(output.find("Смауг") != std::string::npos);
}

TEST(EditorTest, SaveAndLoad) {
    MockObserver observer;
    DungeonEditor editor(&observer);
    
    editor.addNPC(KNIGHT, "Артур", 100, 200);
    editor.addNPC(DRAGON, "Смауг", 300, 400);
    editor.addNPC(PEGASUS, "Пегас", 50, 150);
    
    const std::string testFile = "test_save.txt";
    editor.save(testFile);
    EXPECT_TRUE(observer.contains("Сохранено"));
    
    DungeonEditor editor2(&observer);
    editor2.load(testFile);
    EXPECT_EQ(editor2.getNPCs().size(), 3);
    EXPECT_TRUE(observer.contains("Загружено"));
    
    auto& npcs = editor2.getNPCs();
    bool foundArthur = false;
    bool foundSmaug = false;
    
    for (const auto& npc : npcs) {
        if (npc->getName() == "Артур" && npc->getType() == KNIGHT) {
            foundArthur = true;
        }
        if (npc->getName() == "Смауг" && npc->getType() == DRAGON) {
            foundSmaug = true;
        }
    }
    
    EXPECT_TRUE(foundArthur);
    EXPECT_TRUE(foundSmaug);
    
    std::remove(testFile.c_str());
}

TEST(EditorTest, BattleThroughEditor) {
    MockObserver observer;
    DungeonEditor editor(&observer);
    
    editor.addNPC(KNIGHT, "Артур", 0, 0);
    editor.addNPC(DRAGON, "Смауг", 5, 0);
    
    editor.startBattle(10);
    EXPECT_TRUE(observer.countContains("бой") > 0 || 
                observer.countContains("БОЙ") > 0);
    EXPECT_LT(editor.getNPCs().size(), 2);
    
    editor.startBattle(10);
    EXPECT_TRUE(observer.contains("нужно минимум"));
}

TEST(EditorTest, BoundaryCoordinates) {
    MockObserver observer;
    DungeonEditor editor(&observer);
    
    editor.addNPC(KNIGHT, "Мин", 0, 0);
    editor.addNPC(DRAGON, "Макс", 500, 500);
    EXPECT_EQ(editor.getNPCs().size(), 2);
    
    editor.addNPC(PEGASUS, "Вне", -1, 501);
    EXPECT_EQ(editor.getNPCs().size(), 2);
    EXPECT_TRUE(observer.contains("диапазона"));
}

// ===================== Тесты Observer =====================
TEST(ObserverTest, ConsoleObserver) {
    ConsoleObserver observer;
    
    testing::internal::CaptureStdout();
    observer.update("Тестовое сообщение");
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Тестовое сообщение") != std::string::npos);
    EXPECT_TRUE(output.find("[СОБЫТИЕ]") != std::string::npos);
}

TEST(ObserverTest, FileObserver) {
    const std::string testFile = "test_log.txt";
    
    {
        FileObserver observer(testFile);
        observer.update("Первое сообщение");
        observer.update("Второе сообщение");
        observer.update("Третье сообщение");
        EXPECT_TRUE(observer.isFileOpen());
    }
    
    std::ifstream file(testFile);
    ASSERT_TRUE(file.is_open());
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    EXPECT_TRUE(content.find("Первое сообщение") != std::string::npos);
    EXPECT_TRUE(content.find("Второе сообщение") != std::string::npos);
    EXPECT_TRUE(content.find("ЛОГ СИМУЛЯЦИИ") != std::string::npos);
    
    file.close();
    std::remove(testFile.c_str());
}

TEST(ObserverTest, InvalidFileName) {
    FileObserver observer("/несуществующая/директория/log.txt");
    EXPECT_FALSE(observer.isFileOpen());
}

// ===================== Тесты Fight Logic =====================
TEST(FightTest, KnightVsDragon) {
    Knight knight(0, 0, "Рыцарь");
    Dragon dragon(0, 0, "Дракон");
    
    int knightWins = 0;
    int totalTests = 100;
    
    for (int i = 0; i < totalTests; i++) {
        Knight testKnight(0, 0, "Рыцарь");
        Dragon testDragon(0, 0, "Дракон");
        
        if (testKnight.fight(&testDragon)) {
            knightWins++;
        }
    }
    
    EXPECT_GT(knightWins, 30); // Рыцарь должен побеждать часто
    EXPECT_LT(knightWins, 90);
}

TEST(FightTest, DragonVsPegasus) {
    Dragon dragon(0, 0, "Дракон");
    Pegasus pegasus(0, 0, "Пегас");
    
    int dragonWins = 0;
    int totalTests = 100;
    
    for (int i = 0; i < totalTests; i++) {
        Dragon testDragon(0, 0, "Дракон");
        Pegasus testPegasus(0, 0, "Пегас");
        
        if (testDragon.fight(&testPegasus)) {
            dragonWins++;
        }
    }
    
    EXPECT_GT(dragonWins, 60); // Дракон имеет преимущество
    EXPECT_LT(dragonWins, 100);
}

TEST(FightTest, PegasusVsKnight) {
    Pegasus pegasus(0, 0, "Пегас");
    Knight knight(0, 0, "Рыцарь");
    
    int pegasusWins = 0;
    int totalTests = 100;
    
    for (int i = 0; i < totalTests; i++) {
        Pegasus testPegasus(0, 0, "Пегас");
        Knight testKnight(0, 0, "Рыцарь");
        
        if (testPegasus.fight(&testKnight)) {
            pegasusWins++;
        }
    }
    
    EXPECT_GT(pegasusWins, 40); // Пегас имеет небольшое преимущество
    EXPECT_LT(pegasusWins, 80);
}

// ===================== Интеграционные тесты =====================
TEST(IntegrationTest, FullEditorWorkflow) {
    MockObserver observer;
    DungeonEditor editor(&observer);
    
    // 1. Добавляем NPC
    editor.addNPC(KNIGHT, "Артур", 100, 100);
    editor.addNPC(DRAGON, "Смауг", 150, 150);
    editor.addNPC(PEGASUS, "Пегас", 120, 120);
    EXPECT_EQ(editor.getNPCs().size(), 3);
    
    // 2. Показываем
    testing::internal::CaptureStdout();
    editor.showAll();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Артур") != std::string::npos);
    
    // 3. Сохраняем
    const std::string testFile = "integration_test.txt";
    editor.save(testFile);
    EXPECT_TRUE(observer.contains("Сохранено"));
    
    // 4. Запускаем бой
    editor.startBattle(50);
    EXPECT_LT(editor.getNPCs().size(), 3);
    
    // 5. Загружаем сохраненное
    DungeonEditor editor2(&observer);
    editor2.load(testFile);
    EXPECT_EQ(editor2.getNPCs().size(), 3);
    
    std::remove(testFile.c_str());
}

TEST(IntegrationTest, BattleWithMultipleNPCs) {
    MockObserver observer;
    
    // Создаем много NPC
    std::vector<std::shared_ptr<NPC>> npcs;
    for (int i = 0; i < 10; i++) {
        if (i % 3 == 0) {
            npcs.push_back(Factory::create(KNIGHT, "Рыцарь" + std::to_string(i), i*10, i*10));
        } else if (i % 3 == 1) {
            npcs.push_back(Factory::create(DRAGON, "Дракон" + std::to_string(i), i*10, i*10));
        } else {
            npcs.push_back(Factory::create(PEGASUS, "Пегас" + std::to_string(i), i*10, i*10));
        }
    }
    
    // Запускаем бой
    BattleVisitor battle(30, &observer);
    battle.startBattle(npcs);
    
    // После боя должно остаться меньше NPC
    EXPECT_LT(npcs.size(), 10);
    EXPECT_GT(npcs.size(), 0); // Хотя бы один должен выжить
    
    // Проверяем, что все оставшиеся NPC живы
    for (const auto& npc : npcs) {
        EXPECT_TRUE(npc->isAlive());
    }
}

// ===================== Граничные тесты =====================
TEST(BoundaryTest, ZeroDistanceBattle) {
    MockObserver observer;
    
    auto knight = std::make_shared<Knight>(0, 0, "Рыцарь");
    auto dragon = std::make_shared<Dragon>(0, 0, "Дракон"); // Точно в одной точке
    
    std::vector<std::shared_ptr<NPC>> npcs = {knight, dragon};
    BattleVisitor battle(0, &observer); // Дистанция 0
    
    battle.startBattle(npcs);
    
    // NPC в одной точке должны сражаться
    bool oneDead = !knight->isAlive() || !dragon->isAlive();
    EXPECT_TRUE(oneDead);
    EXPECT_TRUE(observer.contains("победил"));
}

TEST(BoundaryTest, NegativeCoordinates) {
    // NPC можно создать с отрицательными координатами
    Knight knight(-100, -200, "Рыцарь");
    EXPECT_EQ(knight.getX(), -100);
    EXPECT_EQ(knight.getY(), -200);
    
    // Но редактор не должен их принимать
    MockObserver observer;
    DungeonEditor editor(&observer);
    editor.addNPC(KNIGHT, "Негатив", -10, -10);
    EXPECT_TRUE(observer.contains("диапазона"));
    EXPECT_EQ(editor.getNPCs().size(), 0);
}

TEST(BoundaryTest, EmptyName) {
    MockObserver observer;
    DungeonEditor editor(&observer);
    
    // Попытка создать NPC с пустым именем
    editor.addNPC(KNIGHT, "", 100, 200);
    EXPECT_EQ(editor.getNPCs().size(), 1); // Должно создаться
}

// ===================== Главная функция =====================
int main(int argc, char **argv) {
    std::cout << "========================================" << std::endl;
    std::cout << "   ТЕСТИРОВАНИЕ РЕДАКТОРА ПОДЗЕМЕЛЬЯ   " << std::endl;
    std::cout << "========================================" << std::endl;
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}