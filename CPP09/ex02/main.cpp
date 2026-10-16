#include "PmergeMe.hpp"
#include <cstdlib>
#include <limits.h>
#include <cctype>

static bool isInvalid(long num)
{
    return (num < 0 || num > INT_MAX);
}

// strtol accetta anche "", " 42" e "+42": li scarto prima
static bool isNumber(const char *str)
{
    if (!str[0])
        return false;
    for (size_t i = 0; str[i]; i++)
    {
        if (!isdigit(static_cast<unsigned char>(str[i])))
            return false;
    }
    return true;
}

int main(int argc, char **argv)
{

    std::vector<int> input;

    if (argc < 3)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    for (int i = 1; argv[i]; i++)
    {
        try
        {
            if (!isNumber(argv[i]))
                throw PmergeMe::InvalidInput();
            char *end;
            long num = std::strtol(argv[i], &end, 10);
            if (isInvalid(num) || *end != '\0')
                throw PmergeMe::InvalidInput();
            int n = static_cast<int>(num);
            input.push_back(n);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return 1;
        }
    }
    try
    {
        PmergeMe test(input);
        std::cout << "Before: ";
        printCont(input);
        test.process();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
