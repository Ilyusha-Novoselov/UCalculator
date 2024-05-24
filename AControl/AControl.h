#ifndef ACONTROL_H
#define ACONTROL_H

#include <string>

namespace UCalculator {
enum State_control {
    // Начальное состояние
    cStart,

    // Ввод и редактирование
    cEditing,

    // Функция вычислена
    FunDone,

    // Функция для одного операнда выполнена
    FuncOneDone,

    // Выполнена команда памяти
    cMemDone,

    // Выражение вычислено
    cExpDone,

    // Операция выполнена
    cOpDone,

    // Ошибка
    cError
};

class AControl
{
public:
    // Выполнить команду редактора (0 - 20)
    virtual std::string DoEditorCommand(int command) = 0;

    // Выполнить команду процессора (0 - 11)
    virtual void DoProcessorCommand(int command) = 0;

    // Выполнить команду памяти
    virtual uint8_t DoMemoryCommand(int command) = 0;

    // Сброс
    virtual void DoReset(bool flag = false) = 0;

    // Получить состояние
    virtual State_control get_state() = 0;

    // Установить состояние
    virtual void set_state(State_control state) = 0;

    // Установить систему счисления
    virtual void set_p(uint8_t p) = 0;

    // Получить систему счисления
    virtual uint8_t get_p() = 0;
};
}

#endif // ACONTROL_H
