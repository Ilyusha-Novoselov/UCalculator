#include "peditor.h"

bool UCalculator::PEditor::isZero() { return _number == "0"; }

void UCalculator::PEditor::addSign() { if(_number[0] != '-') _number.insert(0, "-"); else _number.erase(0, 1); }

void UCalculator::PEditor::addDelim() { if (_number.find('.') == std::string::npos && !_number.empty()) _number += "."; }
