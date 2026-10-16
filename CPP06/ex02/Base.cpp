#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include "Base.hpp"

Base::~Base(){}

Base *generate(void){

    int n;
    n = rand()%3;

    switch (n) {
        case 0: {            
            std::cout << "Initial Class: A" << std::endl;
            return new A();
        }
        case 1: {            
            std::cout << "Initial Class: B" << std::endl;
            return new B();
        }
        case 2:{            
             std::cout << "Initial Class: C" << std::endl;
             return new C();
        }
    }
    return (NULL);
}
void identify(Base *p){
    if (dynamic_cast<A *>(p))
        std::cout << "A\n";
    else if (dynamic_cast<B *>(p))
        std::cout << "B\n";
    else if (dynamic_cast<C *>(p))
        std::cout << "C\n";

}

void identify(Base &p){
    try {
        A &a = dynamic_cast<A &>(p);
        (void)a;
        std::cout << "A\n";
        return;
    }
    catch (const std::exception &e){
        std::cout << e.what()<< std::endl;
    }
    try {
        B &b = dynamic_cast<B &>(p);
        (void)b;
        std::cout << "B\n";
        return;
    }
    catch (const std::exception &e){
        std::cout << e.what()<< std::endl;
    }
    try {
        C &c = dynamic_cast<C &>(p);
        (void)c;
        std::cout << "C\n";
    }
    catch (const std::exception &e){
        std::cout << e.what()<< std::endl;
    }
}