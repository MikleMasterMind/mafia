#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "library/shared_ptr/shared_ptr.h"

// Предполагаем, что реализация TSharedPtr находится в отдельном файле
// #include "TSharedPtr.h"

class TestClass {
public:
    TestClass(int value) : value_(value) {
        std::cout << "Создан объект TestClass с значением " << value_ << std::endl;
    }
    ~TestClass() {
        std::cout << "Уничтожен объект TestClass с значением " << value_ << std::endl;
    }
    void print() const {
        std::cout << "Значение: " << value_ << std::endl;
    }
    int value_;
};

int main() {
    // Создание пустого указателя
    TSharedPtr<TestClass> ptr1;
    std::cout << "Пустой указатель: " << (ptr1 ? "не null" : "null") << std::endl;

    // Создание указателя с объектом
    TSharedPtr<TestClass> ptr2(new TestClass(42));
    ptr2->print();
    std::cout << "Количество ссылок: " << ptr2.use_count() << std::endl;

    // Копирование
    TSharedPtr<TestClass> ptr3 = ptr2;
    std::cout << "После копирования, количество ссылок: " << ptr2.use_count() << std::endl;

    // Перемещение
    TSharedPtr<TestClass> ptr4 = std::move(ptr3);
    std::cout << "После перемещения, ptr3: " << (ptr3 ? "не null" : "null") << std::endl;
    std::cout << "Количество ссылок в ptr4: " << ptr4.use_count() << std::endl;

    // Использование операторов
    if (ptr4) {
        std::cout << "ptr4 не null" << std::endl;
    }
    if (!ptr3) {
        std::cout << "ptr3 null" << std::endl;
    }

    // Сброс указателя
    ptr4.reset();
    std::cout << "После сброса ptr4: " << (ptr4 ? "не null" : "null") << std::endl;

    // Создание через make_shared
    auto ptr5 = make_shared<TestClass>(100);
    ptr5->print();

    // Сравнение
    TSharedPtr<TestClass> ptr6 = make_shared<TestClass>(100);
    std::cout << "Сравнение ptr5 и ptr6: " << (ptr5 == ptr6 ? "равны" : "не равны") << std::endl;

    // Использование с другими типами
    TSharedPtr<int> intPtr = make_shared<int>(42);
    std::cout << "Значение intPtr: " << *intPtr << std::endl;

    // Вектор с shared_ptr
    std::vector<TSharedPtr<TestClass>> vec;
    vec.push_back(make_shared<TestClass>(1));
    vec.push_back(make_shared<TestClass>(2));
    vec.push_back(make_shared<TestClass>(3));

    // Обмен значениями
    TSharedPtr<TestClass> temp = vec[0];
    vec[0].swap(vec[2]);
    std::cout << "После swap: " << vec[0]->value_ << " и " << temp->value_ << std::endl;

    return 0;
}
