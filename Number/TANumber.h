#ifndef TANUMBER_H
#define TANUMBER_H

#include <string>

namespace UCalculator {
class TANumber
{
public:
    virtual ~TANumber() = default;

    // Получить число
    virtual std::string GetNumber(void) const = 0;

    // Установить число
    virtual void SetNumber(std::string newn) = 0;

    // Скопировать число
    virtual TANumber* Copy() = 0;

    // Оператор сложения
    virtual TANumber& operator+ (TANumber& B) = 0;

    // Оператор вычитания
    virtual TANumber& operator- (TANumber& B) = 0;

    // Оператор умножения
    virtual TANumber& operator* (TANumber& B) = 0;

    // Оператор деления
    virtual TANumber& operator/ (TANumber& B) = 0;

    // Оператор сравнения
    virtual bool operator ==(TANumber& B) = 0;

    // Возведение в квадрат
    virtual TANumber& Sqr() = 0;

    // Обратить число
    virtual TANumber& Rev(void) = 0;

    // Проверка на 0
    virtual bool EqZero(void) = 0;
};
}

#endif // TANUMBER_H
