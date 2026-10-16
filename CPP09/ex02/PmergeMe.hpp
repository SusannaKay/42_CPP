#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <deque>
#include <utility>
#include <string>
#include <vector>
#include <exception>
#include <algorithm>
#include <ctime>

class PmergeMe{

    private:

    std::vector<int> _input;

    public:

    PmergeMe();
    PmergeMe(const std::vector<int> &input);
    PmergeMe(const PmergeMe &other);
    ~PmergeMe();
    PmergeMe &operator=(const PmergeMe &other);

    void process();
    std::vector<int> processVector();
    std::deque<int> processDeque();

    class InvalidInput : public std::exception {
        virtual const char *what() const throw();
    };
};

template <typename T>
void printCont(const T &cont)
{
    for (size_t i = 0; i < cont.size(); i++)
    {
        std::cout << cont[i] << " ";
    }
    std::cout << std::endl;
}

#endif
