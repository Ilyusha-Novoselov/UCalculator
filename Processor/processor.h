#ifndef PROCESSOR_H
#define PROCESSOR_H

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
    inline void reset() { _lop_Res.reset(); _rop.reset(); _operation = None; };

    // Сброс операции
    inline void resetOperation() { _operation = None; };

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
    inline T getLop() { return _lop_Res; };

    // Установить левый операнд
    inline void setLop(T& lop) { _lop_Res = lop; };

    // Получить правый операнд
    inline T getRop() { return _rop; };

    // Установить правый операнд
    inline void setRop(T& rop) { _rop = rop; };

    // Получить резерв
    inline T getTemp() { return _temp; };

    // Установить резерв
    inline void setTemp(T& temp) { _temp = temp; };

    // Получить состояние
    inline Operation getOperation() { return _operation; };

    // Установить состояние
    inline void setOperation(const Operation& operation) { _operation = operation; };

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
