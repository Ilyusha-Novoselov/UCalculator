#ifndef AEDITOR_H
#define AEDITOR_H

#include <string>

namespace UCalculator {
class AEditor
{
public:
    // Проверка на 0
    virtual inline bool isZero() = 0;

    // Добавить знак
    virtual inline void addSign() = 0;

    // Добавить цифру
    virtual void addDigit(int n);

    // Добавить ноль
    virtual void addZero();

    // Удалить символ
    virtual void Bs();

    // Очистить редактируемое число
    virtual void Clear();

    // Добавить разделитель
    virtual inline void addDelim() = 0;

    // Получить отредактированное число
    virtual std::string getNumber();

    // Установить число
    virtual void setNumber(std::string& number);

    virtual ~AEditor() = default;

protected:
    std::string _number = "";

};
}

#endif // AEDITOR_H
