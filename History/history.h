#ifndef HISTORY_H_
#define HISTORY_H_

#include <string>
#include <vector>

namespace UCalculator {
template<class T>
	struct Record 
    {
        T lop;
        std::string operation;
        T rop;
        T result;
        uint8_t p;
	};

template<class T>
	class History {
	public:
		// Конструктор
		History() : _allRecords() {};

		// Добавление записи
        inline void AddRecord(T& lop, std::string& operation, T& rop, T& result, uint8_t p) {
            Record<T> record = { lop, operation, rop, result, p };
            _allRecords.push_back(record);
		};

		// Очистка истории
		inline void Clear() { _allRecords.clear(); };

		// Текущий размер списка в числе записей
		inline size_t Count() { return _allRecords.size(); };

        // Получить список истории
        inline std::vector<Record<T>> get_allRecord() { return _allRecords; };

	private:
        // Вектор, содержащий всю историю операций
        std::vector<Record<T>> _allRecords;
	};
}

#endif
