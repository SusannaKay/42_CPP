#include "Cat.hpp"

Cat::Cat() : ideas(new(Brain))
{
	type = "Cat";
	std::cout << "Cat default constructor called" << std::endl;
}

Cat::Cat(const Cat& other) : Animal(other), ideas(new(Brain))
{
	std::cout << "Cat copy constructor called" << std::endl;
	*ideas = *other.ideas;
}

Cat& Cat::operator=(const Cat& other)
{
	std::cout << "Cat assignment operator called" << std::endl;
	if (this != &other)
		{
			Animal::operator=(other);
			*ideas = *other.ideas;
		}
	return (*this);
}

Cat::~Cat()
{
	delete ideas;
	std::cout << "Cat destructor called" << std::endl;
}

void	Cat::makeSound() const
{
	std::cout << "Meow meow!" << std::endl;
}

void	Cat::setIdea(const std::string &idea, int i){
	ideas->setIdea(idea, i);
}

std::string Cat::getIdea(int i) const{
	return ideas->getIdea(i);
}