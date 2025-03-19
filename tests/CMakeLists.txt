cmake_minimum_required(VERSION 3.16)

project(PassManagerTests)

enable_testing()

# Подключаем GoogleTest
add_subdirectory(googletest)

# Создаём исполняемый файл тестов
add_executable(UnitTests
    test_main.cpp
)

# Линкуем тесты с GoogleTest и основной библиотекой
target_link_libraries(UnitTests PRIVATE gtest gtest_main Qt6::Core PassManager)

# Добавляем тест в CTest
add_test(NAME CryptoTest COMMAND UnitTests)
