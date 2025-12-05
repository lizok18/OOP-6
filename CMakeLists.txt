cmake_minimum_required(VERSION 3.10)
project(DungeonProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Все исходные файлы
set(SOURCE_FILES
    src/npc.cpp
    src/editor.cpp
    src/factory.cpp
    src/observer.cpp
    src/dragon.cpp
    src/knight.cpp
    src/pegasus.cpp
    src/visitor.cpp
)

# Основное приложение
add_executable(dungeon_editor main.cpp ${SOURCE_FILES})

# Тесты с Google Test (скачиваем автоматически)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

add_executable(dungeon_tests tests/tests.cpp ${SOURCE_FILES})
target_link_libraries(dungeon_tests gtest gtest_main)

# Для запуска тестов через ctest
enable_testing()
add_test(NAME DungeonTests COMMAND dungeon_tests)