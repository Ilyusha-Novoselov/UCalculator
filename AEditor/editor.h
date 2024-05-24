#ifndef EDITOR_H_
#define EDITOR_H_

#include <string>
#include <algorithm>
#include <map>

#include "converter_10_p.h"

namespace UCalculator {
	class Editor {
	public:
        // Проверка на 0
        inline bool isZero() { return _number == "0"; };

        // Добавить знак
        inline void addSign() { if(_number[0] != '-') _number.insert(0, "-"); else _number.erase(0, 1); };

        // Добавить цифру
        inline void addDigit(int n) { _number.push_back(Converter::Converter_10_P::int_to_char(n)); };

		// Добавить ноль
        inline void addZero() { if(_number.empty() || _number == "-") _number += "0."; else _number += "0"; };

        // Удалить символ
        inline void Bs() { if(!_number.empty()) _number.pop_back(); };

        // Очистить редактируемое число
        inline void Clear() { _number.clear(); };

        // Добавить разделитель
        inline void addDelim() { if (_number.find('.') == std::string::npos && !_number.empty()) _number += "."; };

		// Получить отредактированное число
        inline std::string getNumber() { return _number; };

        // Установить число
        inline void setNumber(std::string& number) { _number = number; };

	private:
        // Поле для хранения редактируемого числа
        std::string _number = "";

	};
}

#endif
