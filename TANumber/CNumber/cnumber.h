#ifndef CNUMBER_H
#define CNUMBER_H

#include <stdexcept>
#include <cmath>

#include "pnumber.h"

namespace UCalculator {
class CNumber: public TANumber {
public:
    // Конструкторы
    CNumber() : _number("0+i*0"), _a(), _b(), _p(10) {};

    CNumber(const std::string& number, uint8_t p) : _number(number), _p(p) {
        // Найти позицию знака '+'
        size_t i_pos = number.find('+');

        // Найти позицию знака '*'
        size_t star_pos = number.find('*', i_pos);

        // Извлечь действительную часть (от начала строки до знака 'i')
        _a.SetNumber(number.substr(0, i_pos));

        // Извлечь мнимую часть (от знака '*' до конца строки)
        _b.SetNumber(number.substr(star_pos + 1));
    };

    // Конструктор копирования
    CNumber(const CNumber& other) : _number(other._number), _a(other._a), _b(other._b), _p(other._p) {}

    // Получить число
    virtual std::string GetNumber(void) const override { return _number; }

    // Установить число
    virtual void SetNumber(std::string newn) override { _number = newn; }

    // Скопировать число
    virtual CNumber* Copy() override { return new CNumber(_number, _p); }

    // Оператор присваивания
    const CNumber& operator= (CNumber& B) {
        _number = B._number;
        _a = B._a;
        _b = B._b;
        _p = B._p;
        return *this;
    }

    // Оператор сложения
    virtual CNumber& operator+ (TANumber& B) override {
        CNumber& other = dynamic_cast<CNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform addition");

        // Сложение вещественных и мнимых частей отдельно
        PNumber realResult = _a + other._a;
        PNumber imaginaryResult = _b + other._b;

        // Создание результирующего CNumber
        std::string result = realResult.GetNumber() + "+i*" + imaginaryResult.GetNumber();
        return *(new CNumber(result, _p));
    }

    // Оператор вычитания
    virtual CNumber& operator- (TANumber& B) override {
        CNumber& other = dynamic_cast<CNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform addition");

        // Вычитание вещественных и мнимых частей отдельно
        PNumber realResult = _a - other._a;
        PNumber imaginaryResult = _b - other._b;

        // Создание результирующего CNumber
        std::string result = realResult.GetNumber() + "+i*" + imaginaryResult.GetNumber();
        return *(new CNumber(result, _p));
    };

    // Оператор умножения
    virtual CNumber& operator* (TANumber& B) override {
        CNumber& other = dynamic_cast<CNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform addition");

        // (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
        PNumber realResult = (_a * other._a) - (_b * other._b);
        PNumber imaginaryResult = (_a * other._b) + (_b * other._a);

        // Создание результирующего CNumber
        std::string result = realResult.GetNumber() + "+i*" + imaginaryResult.GetNumber();
        return *(new CNumber(result, _p));
    };

    // Оператор деления
    virtual CNumber& operator/ (TANumber& B) override {
        CNumber& other = dynamic_cast<CNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform addition");

        // (a + bi) / (c + di) = (ac + bd) / (c^2 + d^2) + ((bc - ad) / (c^2 + d^2))i
        PNumber denominator = (other._a * other._a) + (other._b * other._b);

        if (denominator.EqZero()) {
            throw std::domain_error("Деление на ноль");
        }

        PNumber realResult = ((_a * other._a) + (_b * other._b)) / denominator;
        PNumber imaginaryResult = ((_b * other._a) - (_a * other._b)) / denominator;

        // Создание результирующего CNumber
        std::string result = realResult.GetNumber() + "+i*" + imaginaryResult.GetNumber();
        return *(new CNumber(result, _p));
    };

    // Оператор сравнения
    virtual bool operator ==(TANumber& B) override {
        CNumber& other = dynamic_cast<CNumber&>(B);

        // Проверяем совпадение оснований
        if (_p != other._p) {
            return false;
        }

        return (_a == other._a && _b == other._b);
    };

    // Возведение в квадрат
    virtual CNumber& Sqr() override {
        return *this * *this;
    };

    // Обратить число
    virtual CNumber& Rev(void) override {
        // Вычисляем модуль (a^2 + b^2)
        PNumber modulusSquared = (_a * _a) + (_b * _b);

        // Проверяем деление на ноль
        if (modulusSquared.EqZero()) {
            throw std::domain_error("Деление на ноль");
        }

        // Создаем сопряженное число (a - bi)
        PNumber conjReal = _a;
        PNumber minus("-1", 10);
        PNumber conjImaginary = _b * minus;

        // Формируем строку для сопряженного числа
        std::string conjNumber = conjReal.GetNumber() + "+i*" + conjImaginary.GetNumber();

        // Создаем новый объект для сопряженного числа
        CNumber conjugate(conjNumber, _p);

        auto temp = CNumber(modulusSquared.GetNumber() + "+i*0", _p);

        // Делим сопряженное число на модуль
        return conjugate / temp;
    };

    // Проверка на 0
    virtual bool EqZero(void) override {
        if(_number != "0+i*0")
            return false;
        return true;
    };

    // Взять основние P
    uint8_t get_p() const { return _p; }

    // Установить основние P
    void set_p(const uint8_t p) { _p = p; }

    // Перевести в другую систему счисления
    void turnTo(uint8_t p)
    {
        _a.turnTo(p);
        _b.turnTo(p);
        _number = _a.GetNumber() + "+i*" + _b.GetNumber();
        _p = p;
    }

    void reset()
    {
        _number = "0+i*0";
        _p = 10;
        _a.reset();
        _b.reset();
    }

    // Вычисление модуля
    PNumber Modulus() {
        // Вычисляем квадрат модуля (a^2 + b^2)
        PNumber modulusSquared = (_a * _a) + (_b * _b);

        // Возвращаем квадратный корень из модуля
        return modulusSquared.Sqrt();
    }

    // Метод для вычисления аргумента
    PNumber Argument() {
        if(_p != 10)
            turnTo(10);

        // Преобразуем вещественную и мнимую части в тип double
        double realPart = std::stod(_a.GetNumber());
        double imaginaryPart = std::stod(_b.GetNumber());

        if(_p != 10)
            turnTo(_p);

        PNumber result(std::to_string(std::atan2(imaginaryPart, realPart)), 10);
        result.turnTo(_p);

        // Используем функцию atan2 для вычисления аргумента
        return result;
    }

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
