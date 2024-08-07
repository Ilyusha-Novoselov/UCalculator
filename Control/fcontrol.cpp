#include "fcontrol.h"

std::string UCalculator::FControl::DoEditorCommand(int command)
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
}

void UCalculator::FControl::DoProcessorCommand(int command)
{
    std::string edit;
    if(_editor->getNumber().empty())
        edit = "0";
    else
        edit = _editor->getNumber();

    Record<FNumber> t;
    std::string temp_op;
    auto op = _processor->getOperation();
    switch (op) {
    case None: break;
    case Add: temp_op = "+"; break;
    case Sub: temp_op = "-"; break;
    case Mul: temp_op = "*"; break;
    case Dvd: temp_op = "/"; break;
    }

    FNumber number(edit, _p);
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
        t.lop = *_processor->getLop().Copy();
        t.operation = temp_op;
        t.rop = *_processor->getRop().Copy();

        _processor->executeOperation();

        t.result = *_processor->getLop().Copy();
        t.p = _p;
        _history->AddRecord(t.lop, t.operation, t.rop, t.result, t.p);

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

uint8_t UCalculator::FControl::DoMemoryCommand(int command)
{
    FNumber number(_editor->getNumber(), _p);
    std::string str = _memory->get_number().GetNumber();
    _state = cMemDone;
    switch(command)
    {
    case 1:
        _memory->store(number);
        return _p;

    case 2:
        if(_p != _memory->get_number().get_p())
        {
            DoReset();
            _p = _memory->get_number().get_p();
        }
        _editor->setNumber(str);
        return _p;

    case 3:
        _memory->add(number);
        return _p;

    case 4:
        _memory->clear();
        return _p;

    default:
        return _p;
    }
}

void UCalculator::FControl::DoReset(bool flag)
{
    _editor->Clear();
    _processor->reset();
    if(flag)
        _memory->clear();
    _state = cStart;
}
