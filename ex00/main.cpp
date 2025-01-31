#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>

int main(int ac, char** av) {
    std::fstream                inputFile(av[1]);
    std::fstream                csvFile("../data.csv");
    std::map<struct tm, int>    map;
    //std::map<struct tm, int>    data;
    std::string                 line;

    if (ac != 2) {
        std::cerr << "Error: argument required." << std::endl;
        return 1;
    }

    if (!inputFile.is_open() || !csvFile.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }
    getline(inputFile, line);
    int i = 0;
    while (getline(inputFile, line)) {
        BitcoinExchange::parseMap(line, map, i);
        i++
    }
    BitcoinExchange::calculate();

    inputFile.close();
    csvFile.close();

    return 0;
}
