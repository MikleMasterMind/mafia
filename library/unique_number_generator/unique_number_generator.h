#pragma once

#include <unordered_set>
#include <random>


namespace NMafia {
    class UniqueNumberGenerator {
    private:
        std::unordered_set<int> generatedNumbers;
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_int_distribution<> dist;
        int minValue;
        int maxValue;

        // Приватный конструктор для предотвращения создания объекта
        UniqueNumberGenerator(int min, int max);

        // Запрещаем копирование и присваивание
        UniqueNumberGenerator(const UniqueNumberGenerator&) = delete;
        UniqueNumberGenerator& operator=(const UniqueNumberGenerator&) = delete;

    public:
        // Статический метод для получения экземпляра
        static UniqueNumberGenerator& getInstance(int min = 0, int max = 1000);

        // Методы
        int getUniqueNumber();
        void clear();
        bool contains(int number) const;
    };
}
