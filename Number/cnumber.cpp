#include <stdexcept>

#include "cnumber.h"

UCalculator::CNumber::CNumber(const std::string &number, uint8_t p) : _number(number), _p(p) {
    // Найти позицию знака '+'
    size_t i_pos = number.find('+');

    // Найти позицию знака '*'
    size_t star_pos = number.find('*', i_pos);

    if(i_pos != std::string::npos)
    {
        // Извлечь действительную часть (от начала строки до знака 'i')
        _a.SetNumber(number.substr(0, i_pos));
        _a.set_p(p);
        // Извлечь мнимую часть (от знака '*' до конца строки)
        _b.SetNumber(number.substr(star_pos + 1));
        _b.set_p(p);
    }
    else
    {
        _a.SetNumber(number);
        _a.set_p(p);
    }
}

const UCalculator::CNumber &UCalculator::CNumber::operator=(CNumber &B) {
    _number = B._number;
    _a = B._a;
    _b = B._b;
    _p = B._p;
    return *this;
}

UCalculator::CNumber &UCalculator::CNumber::operator+(TANumber &B) {
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

UCalculator::CNumber &UCalculator::CNumber::operator-(TANumber &B) {
    CNumber& other = dynamic_cast<CNumber&>(B);
    if(_p != other._p)
        throw std::runtime_error("Failed to perform addition");

    // Вычитание вещественных и мнимых частей отдельно
    PNumber realResult = _a - other._a;
    PNumber imaginaryResult = _b - other._b;

    // Создание результирующего CNumber
    std::string result = realResult.GetNumber() + "+i*" + imaginaryResult.GetNumber();
    return *(new CNumber(result, _p));
}

UCalculator::CNumber &UCalculator::CNumber::operator*(TANumber &B) {
    CNumber& other = dynamic_cast<CNumber&>(B);
    if(_p != other._p)
        throw std::runtime_error("Failed to perform addition");

    // (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
    PNumber realResult = (_a * other._a) - (_b * other._b);
    PNumber imaginaryResult = (_a * other._b) + (_b * other._a);

    // Создание результирующего CNumber
    std::string result = realResult.GetNumber() + "+i*" + imaginaryResult.GetNumber();
    return *(new CNumber(result, _p));
}

UCalculator::CNumber &UCalculator::CNumber::operator/(TANumber &B) {
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
}

bool UCalculator::CNumber::operator ==(TANumber &B) {
    CNumber& other = dynamic_cast<CNumber&>(B);

    // Проверяем совпадение оснований
    if (_p != other._p) {
        return false;
    }

    return (_a == other._a && _b == other._b);
}

UCalculator::CNumber &UCalculator::CNumber::Rev() {
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
}

bool UCalculator::CNumber::EqZero() {
    if(_number != "0+i*0")
        return false;
    return true;
}

void UCalculator::CNumber::turnTo(uint8_t p)
{
    _a.turnTo(p);
    _b.turnTo(p);
    _number = _a.GetNumber() + "+i*" + _b.GetNumber();
    _p = p;
}

void UCalculator::CNumber::reset()
{
    _number = "0+i*0";
    _p = 10;
    _a.reset();
    _b.reset();
}

UCalculator::PNumber UCalculator::CNumber::Modulus() {
    // Вычисляем квадрат модуля (a^2 + b^2)
    PNumber modulusSquared = (_a * _a) + (_b * _b);

    // Возвращаем квадратный корень из модуля
    return modulusSquared.Sqrt();
}

UCalculator::PNumber UCalculator::CNumber::Argument() {
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
