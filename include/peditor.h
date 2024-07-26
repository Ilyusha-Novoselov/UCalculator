#ifndef PEDITOR_H_
#define PEDITOR_H_

#include "AEditor.h"

namespace UCalculator {
    class PEditor: public AEditor {
	public:
        // Проверка на 0
        virtual bool isZero() override;

        // Добавить знак
        virtual void addSign() override;

        // Добавить разделитель
        virtual void addDelim() override;

	};
}

#endif
