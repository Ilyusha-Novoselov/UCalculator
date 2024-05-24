#include "converter_10_p.h"

namespace Converter {

	char Converter_10_P::int_to_char(int n)
	{
		if (n >= 0 && n <= 9)
			return n + '0';
		if (n >= 10)
			return n + 'A' - 10;
		return -1;
	}

    std::string Converter_10_P::int_to_P(int n, uint8_t p)
	{
		std::string result;
        if(n == 0)
            return "0";
		while (n > 0) 
		{
			char number = int_to_char(n % p);
			if (number != -1)
				result.push_back(number);
			else
				return "";
			n /= p;
		}
		std::reverse(result.begin(), result.end());
		return result;
	}

    std::string Converter_10_P::double_to_P(double n, uint8_t p, uint8_t c)
	{
		std::string result;
		while (n > 0 && c > 0) 
		{
			n *= p;
			int int_part = static_cast<int>(n);
			char number = int_to_char(int_part);
			if (number != -1)
				result.push_back(number);
			else
				return "";
			n -= int_part;
			c--;
        }
        while (!result.empty() && result.back() == '0')
            result.pop_back();
		return result;
	}

    std::string Converter_10_P::Do(double n, uint8_t p, uint8_t c)
	{
		std::string result;
		if (n < 0)
		{
			result.push_back('-');
			n *= (-1);
		}
		int int_part = static_cast<int>(n);
        double fract_part = n - int_part;
        if(c == 0 && fract_part > 0)
            c = 5;
        if(c > 0)
			result += int_to_P(int_part, p) + "." + double_to_P(fract_part, p, c);
		else
			result += int_to_P(int_part, p) + double_to_P(fract_part, p, c);
		return result;
	}
}
