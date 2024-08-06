#ifndef PNUMBER_H
#define PNUMBER_H

#include <stdint.h>
#include <cmath>

#include "TANumber.h"

namespace UCalculator {
class PNumber: public TANumber {
public:
    // Конструкторы
    PNumber() : _number("0"), _p(10), _c(3) {};

    PNumber(const std::string& number, uint8_t p);

    // Конструктор копирования
    PNumber(const PNumber& other) : _number(other._number), _p(other._p), _c(other._c) {}

    // Получить число
    inline virtual std::string GetNumber(void) const override { return _number; }

    // Установить число
    inline virtual void SetNumber(std::string newn) override { _number = newn; }

    // Скопировать число
    inline virtual PNumber* Copy() override { return new PNumber(_number, _p); }

    // Оператор присваивания
    const PNumber& operator= (PNumber& B);

    // Оператор сложения
    virtual PNumber& operator+ (TANumber& B) override;

    // Оператор вычитания
    virtual PNumber& operator- (TANumber& B) override;;

    // Оператор умножения
    virtual PNumber& operator* (TANumber& B) override;;

    // Оператор деления
    virtual PNumber& operator/ (TANumber& B) override;;

    // Оператор сравнения
    virtual bool operator ==(TANumber& B) override;;

    // Возведение в квадрат
    virtual PNumber& Sqr() override;

    // Обратить число
    virtual PNumber& Rev(void) override;

    // Проверка на 0
    virtual bool EqZero(void) override;

    // Взять основние P
    uint8_t get_p() const { return _p; }

    // Установить основние P
    inline void set_p(const uint8_t p) { _p = p; }

    // Перевести в другую систему счисления
    void turnTo(uint8_t p);

    void reset();

    // Извлечение корня
    PNumber& Sqrt();

private:
    // Число в с.сч.
    std::string _number;

    // Основание с.сч.
    uint8_t _p;

    // Точность
    uint8_t _c;
};

}

#endif // PNUMBER_H
