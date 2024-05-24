#ifndef СEDITOR_H_
#define СEDITOR_H_

#include "AEditor.h"

namespace UCalculator {
class СEditor: public AEditor {
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
