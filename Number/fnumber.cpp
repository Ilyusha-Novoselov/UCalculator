#include <stdexcept>

#include "fnumber.h"

UCalculator::FNumber::FNumber(const std::string &number, uint8_t p) {
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
}

const UCalculator::FNumber &UCalculator::FNumber::operator=(FNumber &B) {
    _number = B._number;
    _a = B._a;
    _b = B._b;
    _p = B._p;
    return *this;
}

UCalculator::FNumber &UCalculator::FNumber::operator+(TANumber &B) {
    FNumber& other = dynamic_cast<FNumber&>(B);
    if(_p != other._p)
        throw std::runtime_error("Failed to perform addition");

    // a/b + c/d = (a * d + b * c) / (b * d)
    PNumber numerator1 = _a * other._b; // a * d
    PNumber numerator2 = _b * other._a; // b * c
    PNumber numeratorResult = numerator1 + numerator2; // a * d + b * c
    PNumber denominatorResult = _b * other._b; // b * d

    std::string resultNumber = numeratorResult.GetNumber() + "|" + denominatorResult.GetNumber();
    FNumber* result = new FNumber(resultNumber, _p);

    result->Reduce();

    return *result;
}

UCalculator::FNumber &UCalculator::FNumber::operator-(TANumber &B) {
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
}

UCalculator::FNumber &UCalculator::FNumber::operator*(TANumber &B) {
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
}

UCalculator::FNumber &UCalculator::FNumber::operator/(TANumber &B) {
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
}

bool UCalculator::FNumber::operator ==(TANumber &B) {
    FNumber& other = dynamic_cast<FNumber&>(B);

    // Проверяем совпадение оснований
    if (_p != other._p) {
        return false;
    }

    return (_a == other._a && _b == other._b);
}

UCalculator::FNumber &UCalculator::FNumber::Sqr() {
    FNumber* result = &(*this * *this);

    result->Reduce();

    return *result;
}

UCalculator::FNumber &UCalculator::FNumber::Rev() {
    FNumber one("1|1", _p);

    FNumber* result = &(one / *this);

    result->Reduce();

    return *result;
}

bool UCalculator::FNumber::EqZero() {
    if(_number != "0|1")
        return false;
    return true;
}

void UCalculator::FNumber::turnTo(uint8_t p)
{
    _a.turnTo(p);
    _b.turnTo(p);
    _number = _a.GetNumber() + "|" + _b.GetNumber();
    _p = p;
}

void UCalculator::FNumber::reset()
{
    _number = "0|1";
    _p = 10;
    _a.reset();
    _b.reset();
}

void UCalculator::FNumber::Reduce() {
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

    auto a = _a.GetNumber();
    auto b = _b.GetNumber();
    if(b[0] == '-')
    {
        if(a[0] == '-')
        {
            a.erase(0,1);
            b.erase(0,1);
        }
        else
        {
            b.erase(0,1);
            a.insert(0, "-");
        }
    }

    // Обновляем строку дроби
    _number = a + "|" + b;
}

template<class T>
T UCalculator::FNumber::gcd(T a, T b) {
    while (b != 0) {
        T temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
