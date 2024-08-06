#include "feditor.h"

bool UCalculator::FEditor::isZero() { return _number == "0|1"; }

void UCalculator::FEditor::addSign() { if(_number[0] != '-') _number.insert(0, "-"); else _number.erase(0, 1); }

void UCalculator::FEditor::addDelim() {
    if (_number.find('|') == std::string::npos && !_number.empty())
        _number += "|";
}
