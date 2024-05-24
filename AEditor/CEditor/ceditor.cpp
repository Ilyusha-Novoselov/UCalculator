#include "ceditor.h"

bool UCalculator::СEditor::isZero() { return _number == "0+i*0"; }

void UCalculator::СEditor::addSign() {
    size_t index = _number.find('i');
    if (index != std::string::npos)
    {
        if(_number[index + 2] == '-')
            _number.erase(index + 2, 1);
        else
            _number.insert(index + 2, "-");
    }
    else
    {
        if(_number[0] == '-')
            _number.erase(0, 1);
        else
            _number.insert(0, "-");
    }

}

void UCalculator::СEditor::addDelim() {
    if (_number.find('i') == std::string::npos && !_number.empty())
        _number += "+i*";
}
