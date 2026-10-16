/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 12:36:54 by skayed            #+#    #+#             */
/*   Updated: 2026/03/14 14:30:14 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

ClapTrap::ClapTrap(std::string name)
	: _name(name), _hitPoints(10), _energyPoints(10), _attackDamage(0)
{
	std::cout << _name << " constructor has been called." << std::endl;
}

ClapTrap::ClapTrap(const ClapTrap &other)
{
	_name = other.getName();
	_hitPoints = other.getHit();
	_energyPoints = other.getEnergy();
	_attackDamage = other.getAttack();
	std::cout << _name << " copy constructor has been called." << std::endl;
}

ClapTrap &ClapTrap::operator=(const ClapTrap &other)
{
	if (this != &other)
	{
		_name = other.getName();
		_hitPoints = other.getHit();
		_energyPoints = other.getEnergy();
		_attackDamage = other.getAttack();
	}
	std::cout << _name << " overload operator has been called." << std::endl;
	return (*this);
}

ClapTrap::~ClapTrap()
{
	std::cout << _name << " destructor has been called." << std::endl;
}

std::string ClapTrap::getName() const
{
	return (_name);
}

unsigned int ClapTrap::getHit() const
{
	return (_hitPoints);
}

unsigned int ClapTrap::getEnergy() const
{
	return (_energyPoints);
}

unsigned int ClapTrap::getAttack() const
{
	return (_attackDamage);
}

void ClapTrap::setHit(int n)
{
	_hitPoints = n;
}
void ClapTrap::setEnergy(int n)
{
	_energyPoints = n;
}
void ClapTrap::attack(const std::string &target)
{
	if (_energyPoints > 0 && _hitPoints > 0)
	{
		std::cout << "ClapTrap " << _name << " attacks " << target << " causing " << _attackDamage << " points of damage!" << std::endl;
		_energyPoints--;
		return ;
	}
	else
		std::cout << "ClapTrap " << _name << " can't attack." << std::endl;
}
void ClapTrap::takeDamage(unsigned int amount)
{
	if (amount >= _hitPoints)
		_hitPoints = 0;
	else
		_hitPoints -= amount;
	std::cout << "ClapTrap " << _name << " took " << amount << " points of damage." << std::endl;
}
void ClapTrap::beRepaired(unsigned int amount)
{
	if (_energyPoints > 0 && _hitPoints > 0)
	{
		_hitPoints += amount;
		_energyPoints--;
		std::cout << "ClapTrap " << _name << " repairs by " << amount << " hit points " << std::endl;
		return ;
	}
	std::cout << "ClapTrap " << _name << " can't repair himself." << std::endl;
}