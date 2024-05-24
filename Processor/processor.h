#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <map>

namespace UCalculator {
enum Operation {
    None, Add, Sub, Mul, Dvd
};

enum Function {
    Rev, Sqr
};

template<class T>
class Processor {
public:
    // Конструктор
    Processor() : _lop_Res(), _rop(), _operation(None) {};

    // Сброс процессора
    void reset() { _lop_Res.reset(); _rop.reset(); _operation = None; };

    // Сброс операции
    void resetOperation() { _operation = None; };

    // Выполнить операцию
    void executeOperation() {
        if(_operation == None)
        {
            _lop_Res = _rop;
            _rop.reset();
            _operation = Add;
        }
        switch(_operation)
        {
        case Add:
            _lop_Res = _lop_Res + _rop;
            setOperation(_operation);
            break;

        case Sub:
            _lop_Res = _lop_Res - _rop;
            setOperation(_operation);
            break;

        case Mul:
            _lop_Res = _lop_Res * _rop;
            setOperation(_operation);
            break;

        case Dvd:
            _lop_Res = _lop_Res / _rop;
            setOperation(_operation);
            break;

        default:
            break;
        }
    };

    // Вычислить функцию
    void calculateFunction(Function func) {
        switch(func)
        {
        case Rev:
            _rop = _rop.Rev();
            break;

        case Sqr:
            _rop = _rop.Sqr();
            break;

        default:
            break;
        }
    };

    // Получить левый операнд
    T getLop() { return _lop_Res; };

    // Установить левый операнд
    void setLop(T& lop) { _lop_Res = lop; };

    // Получить правый операнд
    T getRop() { return _rop; };

    // Установить правый операнд
    void setRop(T& rop) { _rop = rop; };

    // Получить резерв
    T getTemp() { return _temp; };

    // Установить резерв
    void setTemp(T& temp) { _temp = temp; };

    // Получить состояние
    Operation getOperation() { return _operation; };

    // Установить состояние
    void setOperation(const Operation& operation) { _operation = operation; };

private:
    // Левый операнд
    T _lop_Res;

    // Правый операнд
    T _rop;

    // Резерв для функций
    T _temp;

    // Операция
    Operation _operation;
};

}

#endif // PROCESSOR_H
