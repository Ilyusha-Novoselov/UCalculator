#ifndef FCONTROL_H
#define FCONTROL_H

#include <memory>

#include "AControl.h"
#include "feditor.h"
#include "fnumber.h"
#include "processor.h"
#include "memory.h"
#include "history.h"

namespace UCalculator {

class FControl: public AControl {
public:
    // Конструктор
    FControl(): _editor(std::make_unique<FEditor>()), _processor(std::make_unique<Processor<FNumber>>()), _memory(std::make_unique<Memory<FNumber>>()),_history(std::make_unique<History<FNumber>>()), _state(cStart), _p(10) {};

    // Выполнить команду редактора (0 - 20)
    virtual std::string DoEditorCommand(int command) override;

    // Выполнить команду процессора (0 - 11)
    virtual void DoProcessorCommand(int command) override;

    // Выполнить команду памяти
    virtual uint8_t DoMemoryCommand(int command) override;

    // Сброс
    virtual void DoReset(bool flag = false) override;

    // Получить состояние
    inline virtual State_control get_state() override { return _state; }

    // Установить состояние
    inline virtual void set_state(State_control state) override { _state = state; }

    // Установить систему счисления
    inline virtual void set_p(uint8_t p) override { _p = p; }

    // Получить систему счисления
    inline virtual uint8_t get_p() override { return _p; }

    // Получить историю
    inline std::vector<Record<FNumber>> get_history() { return _history->get_allRecord(); };

private:
    // Редактор
    std::unique_ptr<FEditor> _editor;

    // Процессор
    std::unique_ptr<Processor<FNumber>> _processor;

    // Память
    std::unique_ptr<Memory<FNumber>> _memory;

    // История
    std::unique_ptr<History<FNumber>> _history;

    // Состояние
    State_control _state = cStart;

    // Система счисления
    uint8_t _p;
};

}

#endif // CONTROL_H
