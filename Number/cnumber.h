#ifndef CNUMBER_H
#define CNUMBER_H

#include <cmath>

#include "pnumber.h"

namespace UCalculator {
class CNumber: public TANumber {
public:
    // Конструкторы
    CNumber() : _number("0+i*0"), _a(), _b(), _p(10) {};

    CNumber(const std::string& number, uint8_t p);

    // Конструктор копирования
    CNumber(const CNumber& other) : _number(other._number), _a(other._a), _b(other._b), _p(other._p) {}

    // Получить число
    inline virtual std::string GetNumber(void) const override { return _number; }

    // Установить число
    inline virtual void SetNumber(std::string newn) override { _number = newn; }

    // Скопировать число
    inline virtual CNumber* Copy() override { return new CNumber(_number, _p); }

    // Оператор присваивания
    const CNumber& operator= (CNumber& B);

    // Оператор сложения
    virtual CNumber& operator+ (TANumber& B) override;

    // Оператор вычитания
    virtual CNumber& operator- (TANumber& B) override;

    // Оператор умножения
    virtual CNumber& operator* (TANumber& B) override;

    // Оператор деления
    virtual CNumber& operator/ (TANumber& B) override;

    // Оператор сравнения
    virtual bool operator ==(TANumber& B) override;

    // Возведение в квадрат
    inline virtual CNumber& Sqr() override {
        return *this * *this;
    };

    // Обратить число
    virtual CNumber& Rev(void) override;

    // Проверка на 0
    virtual bool EqZero(void) override;

    // Взять основние P
    inline uint8_t get_p() const { return _p; }

    // Установить основние P
    inline void set_p(const uint8_t p) { _p = p; }

    // Перевести в другую систему счисления
    void turnTo(uint8_t p);

    void reset();

    // Вычисление модуля
    PNumber Modulus();

    // Метод для вычисления аргумента
    PNumber Argument();

private:
    // Число
    std::string _number;

    // Вещественная часть
    PNumber _a;

    // Мнимая часть
    PNumber _b;

    // Основание с.сч.
    uint8_t _p;

};

}

#endif // CNUMBER_H
