

template <typename T> Array<T>::Array(): _array(NULL), _size(0){}

template <typename T> Array<T>::Array(unsigned int n) : _array(new T[n]()), _size(n){}

template <typename T> Array<T>::Array(const Array <T>&other) : _array(new T[other.size()]), _size(other.size()){

    for (unsigned int i = 0; i < _size; i++)
          _array[i] = other._array[i];
}

template <typename T> Array<T> &Array<T>::operator=(const Array<T> &other){

    if (this != &other){
        if (_array)
            delete[] _array;
    _size = other.size();
    _array = new T[_size];
    for (unsigned int i = 0; i < _size ; i++)
        _array[i] = other._array[i];
    }
    return (*this);

}

template <typename T> T &Array<T>::operator[](unsigned int n) const{
    if (n >= this->_size)
        throw std::out_of_range("Index out of range.");
    return(_array[n]);
}

template <typename T> Array<T>::~Array(){
    if (_array)
        delete[] _array;
}

template <typename T> unsigned int Array<T>::size() const{
    return (_size);
}