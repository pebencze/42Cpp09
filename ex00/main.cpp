#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>

int main(int ac, char** av) {
    if (ac != 2) {
        std::cerr << "Error: usage: './btc [input file]" << std::endl;
        return 1;
    }

    try {
        BitcoinExchange btc;
        btc.run(av[1]);
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
