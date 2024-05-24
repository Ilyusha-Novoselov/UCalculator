#ifndef CONTROL_H
#define CONTROL_H

#include "pnumber.h"
#include "editor.h"
#include "processor.h"
#include "memory.h"

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
template<class T>
class Control {
public:
    // Конструктор
    Control(): _editor(new Editor()), _processor(new Processor<PNumber>()), _memory(new Memory<PNumber>), _state(cStart) {};

    // Деструктор
    ~Control() {
        delete _editor;
        delete _processor;
        delete _memory;
    }

    // Выполнить команду редактора (0 - 20)
    std::string DoEditorCommand(int command)
    {
        _state = cEditing;

        if (command <= 15 && command != 0)
        {
            _editor->addDigit(command);
            return _editor->getNumber();
        }

        switch (command)
        {
        case 0:
            _editor->addZero();
            return _editor->getNumber();

        case 16:
            _editor->addDelim();
            return _editor->getNumber();

        case 17:
            _editor->Bs();
            return _editor->getNumber();

        case 18:
            _editor->Clear();
            return _editor->getNumber();

        case 19:
            _editor->addSign();
            return _editor->getNumber();

        default:
            return _editor->getNumber();
        }
    };

    // Выполнить команду процессора (0 - 11)
    void DoProcessorCommand(int command)
    {
        std::string edit;
        if(_editor->getNumber().empty())
            edit = "0";
        else
            edit = _editor->getNumber();

        T number(edit, _p);
        std::string str;
        switch(command)
        {
        case 0:
            _processor->reset();
            return;

        case 1:
            _processor->resetOperation();
            return;

        case 2:
            _processor->setOperation(Add);
            _state = cOpDone;
            return;

        case 3:
            _processor->setOperation(Sub);
            _state = cOpDone;
            return;

        case 4:
            _processor->setOperation(Mul);
            _state = cOpDone;
            return;

        case 5:
            _processor->setOperation(Dvd);
            _state = cOpDone;
            return;

        case 6:
            _processor->calculateFunction(Rev);

            // Запоминаем левый операнд
            _processor->setTemp(*_processor->getLop().Copy());

            // Полученный результат записываем в редактор
            str = _processor->getRop().GetNumber();
            _editor->setNumber(str);
            _state = FunDone;
            return;

        case 7:
            _processor->calculateFunction(Sqr);

            // Запоминаем левый операнд
            _processor->setTemp(*_processor->getLop().Copy());

            // Полученный результат записываем в редактор
            str = _processor->getRop().GetNumber();
            _editor->setNumber(str);
            _state = FunDone;
            return;

        case 8:
            _processor->setLop(number);
            _state = cOpDone;
            return;

        case 9:
            _processor->setRop(number);
            _state = cOpDone;
            return;

        case 10:
            _processor->executeOperation();

            // Полученный результат записываем в редактор
            str = _processor->getLop().GetNumber();
            _editor->setNumber(str);

            // Если вычисляли функцию, то возвращаем предыдущий левый операнд
            if(_state == FunDone)
                _processor->setLop(*_processor->getTemp().Copy());
            _state = cExpDone;
            return;

        // Смена системы счисления процессора
        case 11:
            number = *_processor->getLop().Copy();
            number.turnTo(_p);
            _processor->setLop(number);

            number = *_processor->getRop().Copy();
            number.turnTo(_p);
            _processor->setRop(number);

            // Восстанавливаем редактор
            number = *_processor->getLop().Copy();
            str = number.GetNumber();
            _editor->setNumber(str);
            return;
        }
    }

    // Выполнить команду памяти
    T DoMemoryCommand(int command)
    {
        T number(_editor->getNumber(), _p);
        std::string str = _memory->get_number().GetNumber();
        _state = cMemDone;
        switch(command)
        {
        case 1:
            _memory->store(number);
            return _memory->get_number();

        case 2:
            if(_p != _memory->get_number().get_p())
                DoReset();
            _editor->setNumber(str);
            return _memory->get_number();

        case 3:
            _memory->add(number);
            return _memory->get_number();

        case 4:
            _memory->clear();
            return _memory->get_number();

        default:
            return _memory->get_number();
        }
    };

    // Сброс
    void DoReset(bool flag = false) { _editor->Clear(); _processor->reset(); if(flag) _memory->clear(); _state = cStart; }

    // Получить состояние
    State_control get_state() { return _state; }

    // Установить состояние
    void set_state(State_control state) { _state = state; }

    // Установить систему счисления
    void set_p(uint8_t p) { _p = p; }

    // Получить систему счисления
    uint8_t get_p() { return _p; }

private:
    // Редактор
    Editor* _editor;

    // Процессор
    Processor<T>* _processor;

    // Память
    Memory<T>* _memory;

    // Состояние
    State_control _state;

    // Система счисления
    uint8_t _p;
};

}

#endif // CONTROL_H
