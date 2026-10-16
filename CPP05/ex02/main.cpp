#include <iostream>
#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <cstdlib>
#include <ctime>

int main() {

srand(time(NULL));

Bureaucrat boss("Boss", 1);
Bureaucrat intern("Intern", 150);

std::cout << boss << std::endl;
std::cout << intern << std::endl;

std::cout << "\n=== SHRUBBERY ===" << std::endl;
    ShrubberyCreationForm shrub("home");

    intern.signForm(shrub);     // ko
    boss.signForm(shrub);       // ok

    intern.executeForm(shrub);  // ko
    boss.executeForm(shrub);    // ok

std::cout << "\n=== ROBOTOMY ===" << std::endl;

    RobotomyRequestForm robot("Bender");

    boss.signForm(robot);
    boss.executeForm(robot); 


std::cout << "\n=== PRESIDENTIAL PARDON ===" << std::endl;

    PresidentialPardonForm pardon("Marvin");

    intern.signForm(pardon);    // ko
    boss.signForm(pardon);      // ok

    intern.executeForm(pardon); // ko
    boss.executeForm(pardon);   // ok


std::cout << "\n=== SENZA FIRMA ===" << std::endl;

    ShrubberyCreationForm test("error");

    boss.executeForm(test);


return 0;
}
