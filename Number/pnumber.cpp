#include <stdexcept>

#include "pnumber.h"
#include "converter_10_p.h"
#include "converter_p_10.h"

UCalculator::PNumber::PNumber(const std::string &number, uint8_t p) : _number(number), _p(p) {
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
}

const UCalculator::PNumber &UCalculator::PNumber::operator=(PNumber &B) {
    _number = B._number;
    _p = B._p;
    _c = B._c;
    return *this;
}

UCalculator::PNumber &UCalculator::PNumber::operator+(TANumber &B) {
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

UCalculator::PNumber &UCalculator::PNumber::operator-(TANumber &B) {
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
}

UCalculator::PNumber &UCalculator::PNumber::operator*(TANumber &B) {
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
}

UCalculator::PNumber &UCalculator::PNumber::operator/(TANumber &B) {
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
}

bool UCalculator::PNumber::operator ==(TANumber &B) {
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
}

UCalculator::PNumber &UCalculator::PNumber::Sqr() { return *this * *this; }

UCalculator::PNumber &UCalculator::PNumber::Rev() {
    PNumber one("1", _p);
    return one / *this;
}

bool UCalculator::PNumber::EqZero() {
    if(_number != "0")
        return false;
    return true;
}

void UCalculator::PNumber::turnTo(uint8_t p)
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

void UCalculator::PNumber::reset()
{
    _number = "0";
    _p = 10;
    _c = 3;
}

UCalculator::PNumber &UCalculator::PNumber::Sqrt() {
    if(_p != 10)
        turnTo(10);
    double num = sqrt(std::stod(GetNumber()));
    SetNumber(std::to_string(num));
    turnTo(_p);
    return *this;
}
