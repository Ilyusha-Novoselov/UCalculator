#ifndef FEDITOR_H_
#define FEDITOR_H_

#include "AEditor.h"

namespace UCalculator {
class FEditor: public AEditor {
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
