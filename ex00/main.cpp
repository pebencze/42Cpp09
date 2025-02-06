#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>

int main(int ac, char** av) {
    if (ac != 2) {
        std::cerr << "Error: argument required." << std::endl;
        return 1;
    }

    std::ifstream	inputFile(av[1]);
    std::ifstream	csvFile("../data.csv");
    std::string		line;
	BitcoinExchange btc;

    if (!inputFile.is_open() || !csvFile.is_open()) {
        std::cerr << "Error: could not open a file." << std::endl;
        return 1;
    }

	btc.parseMap(std::ifstream &inputFile);
	btc.calculate();


    inputFile.close();
    csvFile.close();

    return 0;
}
