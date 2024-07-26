#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <map>

namespace UCalculator {
    enum State {
        On, Off
    };

    template<class T>
    class Memory {
    public:
        // Конструктор
        Memory() : _number(), _state() {};

        // Записать (MS)
        void store(T& number) { _number = number; _state = On; };

        // Получить число (MR)
        T get_number() { return _number; _state = On;};

        // Очистить (т.е. вернуть к начальному состоянию) (MC)
        void clear() { _number.reset(); _state = Off; };

        // Добавить (MA)
        void add(T& number) { if(_number.get_p() == number.get_p()) _number = _number + number; };

        // Получить состояние
        State get_state() { return _state; };

        // Установить состояние
        void set_state(State state) { _state = state; };

        static inline std::map<std::string, int> commandMap = {
            {"MS", 1}, {"MR", 2}, {"M+", 3}, {"MC", 4}
        };

    private:
        // Параметризированное число
        T _number;

        // Состояние
        State _state;
    };

}

#endif // MEMORY_H
