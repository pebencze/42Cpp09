
#include <iostream>
#include "RPN.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Error: usage: ./RPN [expression]" << std::endl;
        return 1;
    }

    try {
        RPN rpn;
        rpn.calculate(av[1]);
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}