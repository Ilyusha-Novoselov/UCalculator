#ifndef FNUMBER_H
#define FNUMBER_H

#include <stdexcept>
#include <cmath>
#include <algorithm>

#include "pnumber.h"

namespace UCalculator {
class FNumber: public TANumber {
public:
    // Конструкторы
    FNumber() : _a(), _b("1", 10), _p(10) {};

    FNumber(const std::string& number, uint8_t p) {
        _number = number;
        _p = p;
        size_t delimiterPos = number.find('|');
        if (delimiterPos != std::string::npos)
        {
            _a.SetNumber(number.substr(0, delimiterPos));
            _a.set_p(p);

            _b.SetNumber(number.substr(delimiterPos + 1));
            if(number.substr(delimiterPos + 1) == "0")
                _b.SetNumber("1");
            _b.set_p(p);
        }
        else
        {
            _a.SetNumber(number);
            _b.SetNumber("1");
            _a.set_p(p);
            _b.set_p(p);
        }
    };

    // Конструктор копирования
    FNumber(const FNumber& other) : _number(other._number), _a(other._a), _b(other._b), _p(other._p) {}

    // Получить число
    virtual std::string GetNumber(void) const override { return _number; }

    // Установить число
    virtual void SetNumber(std::string newn) override { _number = newn; }

    // Скопировать число
    virtual FNumber* Copy() override { return new FNumber(_number, _p); }

    // Оператор присваивания
    const FNumber& operator= (FNumber& B) {
        _number = B._number;
        _a = B._a;
        _b = B._b;
        _p = B._p;
        return *this;
    }

    // Оператор сложения
    virtual FNumber& operator+ (TANumber& B) override {
        FNumber& other = dynamic_cast<FNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform addition");

        // a/b + c/d = (a * d + b * c) / (b * d)
        PNumber numerator1 = _a * other._b; // a * d
        PNumber numerator2 = _b * other._a; // b * c
        PNumber numeratorResult = numerator1 + numerator2; // a * d + b * c
        PNumber denominatorResult = _b * other._b; // b * d

        // Создаем новый объект FNumber для результата
        std::string resultNumber = numeratorResult.GetNumber() + "|" + denominatorResult.GetNumber();
        FNumber* result = new FNumber(resultNumber, _p);

        result->Reduce();

        return *result;
    }

    // Оператор вычитания
    virtual FNumber& operator- (TANumber& B) override {
        FNumber& other = dynamic_cast<FNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform addition");

        // a/b - c/d = (a * d - b * c) / (b * d)
        PNumber numerator1 = _a * other._b; // a * d
        PNumber numerator2 = _b * other._a; // b * c
        PNumber numeratorResult = numerator1 - numerator2; // a * d - b * c
        PNumber denominatorResult = _b * other._b; // b * d

        // Создаем новый объект FNumber для результата
        std::string resultNumber = numeratorResult.GetNumber() + "|" + denominatorResult.GetNumber();
        FNumber* result = new FNumber(resultNumber, _p);

        result->Reduce();

        return *result;
    };

    // Оператор умножения
    virtual FNumber& operator* (TANumber& B) override {
        FNumber& other = dynamic_cast<FNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform addition");

        // a/b * c/d = (a * c) / (b * d)
        PNumber numeratorResult = _a * other._a; // a * c
        PNumber denominatorResult = _b * other._b; // b * d

        // Создаем новый объект FNumber для результата
        std::string resultNumber = numeratorResult.GetNumber() + "|" + denominatorResult.GetNumber();
        FNumber* result = new FNumber(resultNumber, _p);

        result->Reduce();

        return *result;
    };

    // Оператор деления
    virtual FNumber& operator/ (TANumber& B) override {
        FNumber& other = dynamic_cast<FNumber&>(B);
        if(_p != other._p)
            throw std::runtime_error("Failed to perform addition");

        // a/b / c/d = (a * d) / (b * c)
        PNumber numeratorResult = _a * other._b; // a * d
        PNumber denominatorResult = _b * other._a; // b * c

        // Создаем новый объект FNumber для результата
        std::string resultNumber = numeratorResult.GetNumber() + "|" + denominatorResult.GetNumber();
        FNumber* result = new FNumber(resultNumber, _p);

        result->Reduce();

        return *result;
    };

    // Оператор сравнения
    virtual bool operator ==(TANumber& B) override {
        FNumber& other = dynamic_cast<FNumber&>(B);

        // Проверяем совпадение оснований
        if (_p != other._p) {
            return false;
        }

        return (_a == other._a && _b == other._b);
    };

    // Возведение в квадрат
    virtual FNumber& Sqr() override {
        FNumber* result = &(*this * *this);

        result->Reduce();

        return *result;
    };

    // Обратить число
    virtual FNumber& Rev(void) override {
        FNumber one("1|1", _p);

        FNumber* result = &(one / *this);

        result->Reduce();

        return *result;
    };

    // Проверка на 0
    virtual bool EqZero(void) override {
        if(_number != "0|1")
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
        _number = _a.GetNumber() + "|" + _b.GetNumber();
        _p = p;
    }

    void reset()
    {
        _number = "0|1";
        _p = 10;
        _a.reset();
        _b.reset();
    }

    template <class T>
    T gcd(T a, T b) {
        while (b != 0) {
            T temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // Метод для сокращения дроби
    void Reduce() {
        if(_p != 10)
        {
            _a.turnTo(10);
            _b.turnTo(10);
        }
        // Получаем числитель и знаменатель в виде целых чисел
        int64_t numerator = std::stoi(_a.GetNumber());
        int64_t denominator = std::stoi(_b.GetNumber());

        // Находим НОД числителя и знаменателя
        int64_t nod = gcd<int64_t>(numerator, denominator);

        // Делим числитель и знаменатель на НОД
        numerator /= nod;
        denominator /= nod;

        // Устанавливаем сокращенные числитель и знаменатель
        _a.SetNumber(std::to_string(numerator));
        _b.SetNumber(std::to_string(denominator));

        if(_p != 10)
        {
            _a.turnTo(_p);
            _b.turnTo(_p);
        }

        // Обновляем строку дроби
        _number = _a.GetNumber() + "|" + _b.GetNumber();
    }


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
