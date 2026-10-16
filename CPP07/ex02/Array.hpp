#ifndef ARRAY_HPP
# define ARRAY_HPP

#include <stdexcept>


template <typename T>
class Array{

    
    private:

    T   *_array;
    unsigned int _size;
    
    public:

    Array();
    Array(unsigned int n);
    Array(const Array &other);
    Array &operator=(const Array &other);
    T &operator[](unsigned int n) const;
    ~Array();

    unsigned int size() const;
};
#include "Array.tpp"
#endif