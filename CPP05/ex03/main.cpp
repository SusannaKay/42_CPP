
#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <cstdlib>
#include <ctime>
#include "Intern.hpp"

int main() {

srand(time(NULL));

Intern someRandomIntern;


std::cout << "TEST NOME SBAGLIATO" << std::endl;
try{
    AForm *form;
    form = someRandomIntern.makeForm("shrubry creation", "home");
    delete form;
}
catch (std::exception &e)
{
    std::cout << e.what() << std::endl;
}
std::cout << "TEST SHRUBBERY" << std::endl;
try{
    AForm *form;
    form = someRandomIntern.makeForm("shrubbery creation", "home");
    delete form;
}
catch (std::exception &e)
{
    std::cout << e.what() << std::endl;
}

std::cout << "TEST PRESIDENTIAL" << std::endl;
try{
    AForm *form;
    form = someRandomIntern.makeForm("presidential pardon", "home");
    delete form;
}
catch (std::exception &e)
{
    std::cout << e.what() << std::endl;
}

std::cout << "TEST ROBOTOMY" << std::endl;
try{
    AForm *form;
    form = someRandomIntern.makeForm("robotomy request", "home");
    delete form;
}
catch (std::exception &e)
{
    std::cout << e.what() << std::endl;
}

return 0;
}
