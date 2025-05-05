
#include "PmergeMe.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv) {
    try {
        PmergeMe pmergeMe(argc, argv);
        pmergeMe.run();
    } catch(std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
