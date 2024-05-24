#include "AEditor.h"
#include "converter_10_p.h"

void UCalculator::AEditor::addDigit(int n) {
    _number.push_back(Converter::Converter_10_P::int_to_char(n));
}

void UCalculator::AEditor::addZero() {
    if(_number.empty() || _number == "-")
        _number += "0.";
    else
        _number += "0";
}

void UCalculator::AEditor::Bs() {
    if(!_number.empty())
        _number.pop_back();
}

void UCalculator::AEditor::Clear() {
    _number.clear();
}

std::string UCalculator::AEditor::getNumber() {
    return _number;
}

void UCalculator::AEditor::setNumber(std::string &number) {
    _number = number;
}
