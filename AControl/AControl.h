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
    virtual State_control get_state() { return _state; }

    // Установить состояние
    virtual void set_state(State_control state) { _state = state; }

    // Установить систему счисления
    virtual void set_p(uint8_t p) { _p = p; }

    // Получить систему счисления
    virtual uint8_t get_p() { return _p; }

protected:
    // Состояние
    State_control _state = cStart;

    // Система счисления
    uint8_t _p;

};
}

#endif // ACONTROL_H
