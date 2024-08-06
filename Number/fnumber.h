#ifndef FNUMBER_H
#define FNUMBER_H

#include <cmath>

#include "pnumber.h"

namespace UCalculator {
class FNumber: public TANumber {
public:
    // Конструкторы
    FNumber() : _a(), _b("1", 10), _p(10) {};

    FNumber(const std::string& number, uint8_t p);

    // Конструктор копирования
    FNumber(const FNumber& other) : _number(other._number), _a(other._a), _b(other._b), _p(other._p) {}

    // Получить число
    inline virtual std::string GetNumber(void) const override { return _number; }

    // Установить число
    inline virtual void SetNumber(std::string newn) override { _number = newn; }

    // Скопировать число
    virtual FNumber* Copy() override { return new FNumber(_number, _p); }

    // Оператор присваивания
    const FNumber& operator= (FNumber& B);

    // Оператор сложения
    virtual FNumber& operator+ (TANumber& B) override;

    // Оператор вычитания
    virtual FNumber& operator- (TANumber& B) override;

    // Оператор умножения
    virtual FNumber& operator* (TANumber& B) override;

    // Оператор деления
    virtual FNumber& operator/ (TANumber& B) override;

    // Оператор сравнения
    virtual bool operator ==(TANumber& B) override;

    // Возведение в квадрат
    virtual FNumber& Sqr() override;

    // Обратить число
    virtual FNumber& Rev(void) override;

    // Проверка на 0
    virtual bool EqZero(void) override;

    // Взять основние P
    inline uint8_t get_p() const { return _p; }

    // Установить основние P
    inline void set_p(const uint8_t p) { _p = p; }

    // Перевести в другую систему счисления
    void turnTo(uint8_t p);

    void reset();

    template <class T>
    T gcd(T a, T b);

    // Метод для сокращения дроби
    void Reduce();


private:
    // Дробь в виде строки
    std::string _number;

    // Числитель
    PNumber _a;

    // Знаменатель
    PNumber _b;

    // Основание с.сч.
    uint8_t _p;

};

}

#endif // FNUMBER_H
