#include <cmath>

#include "converter_p_10.h"

namespace Converter {

	double Converter_P_10::char_to_num(char ch)
	{
		if (ch >= '0' && ch <= '9')
            return ch - '0';
        if (ch >= 'A' && ch <= 'F')
            return ch - 'A' + 10;
		return -1;
	}

    double Converter_P_10::Do(const std::string& P_num, uint8_t P)
    {
        double weight;
        bool znak = false;
        std::string P_abs_num = P_num;
        if(P_abs_num[0] == '-')
        {
            P_abs_num = P_abs_num.substr(1);
            znak = true;
        }
        std::string p_copy = P_abs_num;
        size_t dotPosition = P_abs_num.find('.');
        if (dotPosition == std::string::npos)
            weight = pow(P, P_abs_num.length() - 1);
        else
        {
            weight = pow(P, dotPosition - 1);
            p_copy.erase(dotPosition, 1);
        }
        return convert(p_copy, P, weight, znak);
    }

    double Converter_P_10::convert(std::string& P_num, uint8_t P, double weight, bool znak)
    {
        double result = 0.0;
        for (char& ch : P_num)
        {
            double number = char_to_num(ch);
            if (number != -1)
                result += number * weight;
            else
                return -1.0;
            weight /= P;
        }
        if(znak)
            result *= (-1);
        return result;
    }
}


