#include "unique_number_generator.h"
#include <stdexcept>


namespace NMafia {
    static UniqueNumberGenerator* instance = nullptr;

    UniqueNumberGenerator::UniqueNumberGenerator(int min, int max)
        : minValue(min), maxValue(max), gen(rd()), dist(min, max) {}

    UniqueNumberGenerator& UniqueNumberGenerator::getInstance(int min, int max) {
        if (!instance) {
            instance = new UniqueNumberGenerator(min, max);
        }
        return *instance;
    }

    int UniqueNumberGenerator::getUniqueNumber() {
        if (generatedNumbers.size() >= static_cast<size_t>(maxValue - minValue + 1)) {
            throw std::runtime_error("Все возможные числа уже сгенерированы");
        }

        int number;
        do {
            number = dist(gen);
        } while (generatedNumbers.count(number) > 0);

        generatedNumbers.insert(number);
        return number;
    }

    void UniqueNumberGenerator::clear() {
        generatedNumbers.clear();
    }

    // Проверка наличия числа
    bool UniqueNumberGenerator::contains(int number) const {
        return generatedNumbers.count(number) > 0;
    }
}
