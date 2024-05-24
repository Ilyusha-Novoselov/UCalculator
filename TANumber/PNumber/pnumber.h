#ifndef PNUMBER_H
#define PNUMBER_H

#include <stdexcept>
#include <cmath>

#include "converter_10_p.h"
#include "converter_p_10.h"
#include "TANumber.h"

namespace UCalculator {
class PNumber: public TANumber {
public:
    // Конструкторы
    PNumber() : _number("0"), _p(10), _c(3) {};

    PNumber(const std::string& number, uint8_t p) : _number(number), _p(p) {
        size_t dotIndex = number.find('.');
        if (dotIndex != std::string::npos)
        {
            while (_number.back() == '0')
                _number.pop_back();
            if(_number.back() == '.')
                _number.pop_back();
            _c = number.substr(dotIndex + 1).length();
        }
        else
            _c = 0;
    };

    // Конструктор копирования
    PNumber(const PNumber& other) : _number(other._number), _p(other._p), _c(other._c) {}

    // Получить число
    virtual std::string GetNumber(void) const override { return _number; }

    // Установить число
    virtual void SetNumber(std::string newn) override { _number = newn; }

    // Скопировать число
    virtual PNumber* Copy() override { return new PNumber(_number, _p); }

    // Оператор присваивания
    const PNumber& operator= (PNumber& B) {
        _number = B._number;
        _p = B._p;
        _c = B._c;
        return *this;
    }

    // Оператор сложения
    virtual PNumber& operator+ (TANumber& B) override {
        PNumber& other = dynamic_cast<PNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform addition");
        PNumber l_op = *this;
        PNumber r_op = other;
        l_op.turnTo(10);
        r_op.turnTo(10);
        PNumber* result = new PNumber(std::to_string(std::stod(l_op._number) + std::stod(r_op._number)), 10);
        result->turnTo(_p);
        return *result;
    }

    // Оператор вычитания
    virtual PNumber& operator- (TANumber& B) override {
        PNumber& other = dynamic_cast<PNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform subtraction");
        PNumber l_op = *this;
        PNumber r_op = other;
        l_op.turnTo(10);
        r_op.turnTo(10);
        PNumber* result = new PNumber(std::to_string(std::stod(l_op._number) - std::stod(r_op._number)), 10);
        result->turnTo(_p);
        return *result;
    };

    // Оператор умножения
    virtual PNumber& operator* (TANumber& B) override {
        PNumber& other = dynamic_cast<PNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform multiplication");
        PNumber l_op = *this;
        PNumber r_op = other;
        l_op.turnTo(10);
        r_op.turnTo(10);
        PNumber* result = new PNumber(std::to_string(std::stod(l_op._number) * std::stod(r_op._number)), 10);
        result->turnTo(_p);
        return *result;
    };

    // Оператор деления
    virtual PNumber& operator/ (TANumber& B) override {
        PNumber& other = dynamic_cast<PNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform division");
        PNumber l_op = *this;
        PNumber r_op = other;
        l_op.turnTo(10);
        r_op.turnTo(10);
        PNumber* result = new PNumber(std::to_string(std::stod(l_op._number) / std::stod(r_op._number)), 10);
        result->turnTo(_p);
        return *result;
    };

    // Оператор сравнения
    virtual bool operator ==(TANumber& B) override {
        PNumber& other = dynamic_cast<PNumber&>(B);

        // Проверяем совпадение оснований
        if (_p != other._p) {
            return false;
        }

        // Приводим оба числа к десятичной системе для сравнения
        PNumber l_op = *this;
        PNumber r_op = other;
        l_op.turnTo(10);
        r_op.turnTo(10);

        // Сравниваем числовые строки в десятичной системе
        return std::stod(l_op._number) == std::stod(r_op._number);
    };

    // Возведение в квадрат
    virtual PNumber& Sqr() override {
        return *this * *this;
    };

    // Обратить число
    virtual PNumber& Rev(void) override {
        PNumber one("1", _p);
        return one / *this;
    };

    // Проверка на 0
    virtual bool EqZero(void) override {
        if(_number != "0")
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
        if(_p == p)
            return;
        if(p == 10)
            _number = std::to_string(Converter::Converter_P_10::Do(_number, _p));
        else
        {
            size_t dotPos = _number.find('.');
            if(dotPos != std::string::npos)
                _c = _number.size() - dotPos - 1;
            else
                _c = 0;
            int k = (int)ceil(log(_p) / log(p));
            double r = Converter::Converter_P_10::Do(_number, _p);
            _number = Converter::Converter_10_P::Do(r, p, _c * k);
        }

        size_t dotPos = _number.find('.');
        if (dotPos != std::string::npos)
        {
            while (_number.back() == '0')
                _number.pop_back();
            if(_number.back() == '.')
                _number.pop_back();
        }

        _p = p;
    }

    void reset()
    {
        _number = "0";
        _p = 10;
        _c = 3;
    }

    // Извлечение корня
    PNumber& Sqrt() {
        if(_p != 10)
            turnTo(10);
        double num = sqrt(std::stod(GetNumber()));
        SetNumber(std::to_string(num));
        turnTo(_p);
        return *this;
    };

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
