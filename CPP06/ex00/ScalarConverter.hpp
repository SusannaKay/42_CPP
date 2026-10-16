#ifndef SCALARCONVERTER_HPP
# define SCALARCONVERTER_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <limits.h>
#include <cstdlib>
#include <cmath>

class ScalarConverter {

	private:
		ScalarConverter();
		ScalarConverter(ScalarConverter &other);
		ScalarConverter &operator=(ScalarConverter &other);
		~ScalarConverter();

	public:
		static void convert (std::string literal);
};

#endif