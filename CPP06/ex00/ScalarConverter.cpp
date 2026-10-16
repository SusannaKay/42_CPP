#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter(){
}
ScalarConverter::ScalarConverter(ScalarConverter &other){
	if (this != &other)
		return; 
}
ScalarConverter &ScalarConverter::operator=(ScalarConverter &other)
{
	if (this != &other)
		return (*this);
	return (*this);
}
ScalarConverter::~ScalarConverter(){

}

static int is_char(std::string literal)
{
if (literal.length() == 1 && !std::isdigit(literal[0]))
		return (1);
	return (0);
}
static int is_impossible(std::string literal)
{
	if (literal.compare("nan") == 0 || literal.compare("nanf") == 0 || literal.compare("-inf")  == 0|| literal.compare("+inf") == 0|| literal.compare("-inff") == 0 || literal.compare("+inff") == 0)
			return(1);
	return(0);
}
static void printChar(double conv, int flag)
{
	if (flag)
		std::cout << "char: impossible" << std::endl;
	else if (conv < 0 || conv > 127)
		std::cout << "char: impossible" << std::endl;
	else if (!std::isprint(static_cast<char>(conv)))
		std::cout << "char: Not Displayable" << std::endl;
	else
		std::cout << "char: '" << static_cast<char>(conv)<< "'"  << std::endl;
}

static void printInt(double conv, int flag)
{
	if (flag)
		std::cout << "int: impossible" << std::endl;
	else if (conv < INT_MIN || conv > INT_MAX)
		std::cout << "int: impossible" << std::endl;
	else
		std::cout << "int: " << static_cast<int>(conv) << std::endl;
}

// ".0" solo se il numero e' intero e cout non lo stampa in notazione scientifica
static bool isRound(double conv)
{
	return (conv == std::floor(conv) && std::fabs(conv) < 1000000);
}

static void printFloat(double conv, int flag)
{
	float f = static_cast<float>(conv);
	std::cout << "float: " << f;
	if (!flag && isRound(conv))
		std::cout << ".0";
	std::cout << "f" << std::endl;
} 
static void printDouble(double conv, int flag)
{
	std::cout << "double: " << conv;
	if (!flag && isRound(conv))
		std::cout << ".0";
	std::cout << std::endl;
}

void ScalarConverter::convert(std::string const literal){

	double conv;
	char *pos;
	int	flag = 0;

	if (literal.empty())
	{
		std::cout << "Invalid input: empty string" << std::endl;
		return;
	}
	if (is_char(literal))
		conv = static_cast<double>(literal[0]);
	else
	{
		flag = is_impossible(literal);
		conv = std::strtod(literal.c_str(), &pos);
		if (pos == literal)
		{
			std::cout << "Invalid input" << std::endl;
			return;
		}
		if (*pos != '\0')
		{
			if (!(*pos == 'f' && *(pos + 1) == '\0'))
			{
				std::cout << "Invalid input" << std::endl;
				return;
			}
		}
	}
	printChar(conv, flag);
	printInt(conv, flag);
	printFloat(conv, flag);
	printDouble(conv, flag);	
}