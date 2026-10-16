#include "Dog.hpp"

Dog::Dog(): ideas(new(Brain))
{
	type = "Dog";
	std::cout << "Dog default constructor called" << std::endl;
}

Dog::Dog(const Dog& other) : Animal(other), ideas(new(Brain))
{
	std::cout << "Dog copy constructor called" << std::endl;
	*ideas = *other.ideas;
}

Dog& Dog::operator=(const Dog& other)
{
	std::cout << "Dog assignment operator called" << std::endl;
	if (this != &other)
	{
		Animal::operator=(other);
		*ideas = *other.ideas;
	}
	return (*this);
}

Dog::~Dog()
{
	delete ideas;
	std::cout << "Dog destructor called" << std::endl;
}

void	Dog::makeSound() const
{
	std::cout << "Woof woof!" << std::endl;
}
void	Dog::setIdea(const std::string &idea, int i){
	ideas->setIdea(idea, i);
}

std::string Dog::getIdea(int i) const{
	return ideas->getIdea(i);
}