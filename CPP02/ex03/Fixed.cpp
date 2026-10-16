/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 10:53:48 by skayed            #+#    #+#             */
/*   Updated: 2026/03/02 10:54:53 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

// Constructor & Destructor Functions

Fixed::Fixed(void)
{
	//std::cout << "Default constructor called" << std::endl;
	this->_rawBits = 0;
}

Fixed::Fixed(int const n)
{
	//std::cout << "Int constructor called" << std::endl;
	this->_rawBits = n * (1 << this->_fractionalBits);
}

Fixed::Fixed(float const n)
{
	//std::cout << "Float constructor called" << std::endl;
	this->_rawBits = roundf(n * (1 << this->_fractionalBits));
}

Fixed::Fixed(const Fixed &other)
{
	//std::cout << "Copy constructor called" << std::endl;
	_rawBits = other.getRawBits();
}

Fixed::~Fixed()
{
	//std::cout << "Destructor called" << std::endl;
}

Fixed &Fixed::operator=(const Fixed &other)
{
	//std::cout << "Copy assignment operator called" << std::endl;
	_rawBits = other.getRawBits();
	return (*this);
}

// Set & Get Functions
int Fixed::getRawBits(void) const
{
	return (this->_rawBits);
}

void Fixed::setRawBits(int const raw)
{
	this->_rawBits = raw;
}

// Conversion Functions
float Fixed::toFloat(void) const
{
	return ((float)this->_rawBits / (1 << this->_fractionalBits));
}
int Fixed::toInt(void) const
{
	return (this->_rawBits >> this->_fractionalBits);
}

//Extra class
std::ostream &operator<<(std::ostream &out, Fixed const &obj)
{
	out << obj.toFloat();
	return (out);
}

// comparison overload functions

bool Fixed::operator>(const Fixed &other) const
{
	return (this->getRawBits() > other.getRawBits());
}
bool Fixed::operator<(const Fixed &other) const
{
	return (this->getRawBits() < other.getRawBits());
}
bool Fixed::operator>=(const Fixed &other) const
{
	return (this->getRawBits() >= other.getRawBits());
}
bool Fixed::operator<=(const Fixed &other) const
{
	return (this->getRawBits() <= other.getRawBits());
}
bool Fixed::operator==(const Fixed &other) const
{
	return (this->getRawBits() == other.getRawBits());
}
bool Fixed::operator!=(const Fixed &other) const
{
	return (this->getRawBits() != other.getRawBits());
}

// Operations overload functions

Fixed Fixed::operator+(const Fixed &other) const
{
	Fixed temp;
	temp.setRawBits(this->getRawBits() + other.getRawBits());
	return (temp);
}
Fixed Fixed::operator-(const Fixed &other) const
{
	Fixed temp;
	temp.setRawBits(this->getRawBits() - other.getRawBits());
	return (temp);
}
Fixed Fixed::operator*(const Fixed &other) const
{
	long long n = static_cast<long long>(this->getRawBits()) * other.getRawBits();
	Fixed temp;
	temp.setRawBits(n >> temp._fractionalBits);
	return (temp);
}
Fixed Fixed::operator/(const Fixed &other) const
{
	Fixed temp;
	long long n = (static_cast<long long>(this->getRawBits()) << temp._fractionalBits) / other.getRawBits();
	temp.setRawBits(n);
	return (temp);
}

// Increment & Decrement Functions

Fixed &Fixed::operator++()
{
	++_rawBits;
	return (*this);
}
Fixed Fixed::operator++(int)
{
	Fixed tmp(*this);
	_rawBits++;
	return (tmp);
}
Fixed &Fixed::operator--()
{
	--_rawBits;
	return (*this);
}
Fixed Fixed::operator--(int)
{
	Fixed tmp(*this);
	_rawBits--;
	return (tmp);
}

// min max functions

const Fixed &Fixed::min(const Fixed &n1, const Fixed &n2)
{
	if (n1 < n2)
		return (n1);
	return (n2);
}
Fixed &Fixed::min(Fixed &n1, Fixed &n2)
{
	if (n1 < n2)
		return (n1);
	return (n2);
}
const Fixed &Fixed::max(const Fixed &n1, const Fixed &n2)
{
	if (n1 > n2)
		return (n1);
	return (n2);
}
Fixed &Fixed::max(Fixed &n1, Fixed &n2)
{
	if (n1 > n2)
		return (n1);
	return (n2);
}