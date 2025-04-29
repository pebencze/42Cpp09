#pragma once
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>
#include <algorithm>

#define CSV_FILE "../data.csv"

class BitcoinExchange {
    public:
		BitcoinExchange();
		~BitcoinExchange();
		BitcoinExchange(BitcoinExchange const & src);
		BitcoinExchange & operator=(BitcoinExchange const & src);

        void parseCsvFile(std::ifstream &csvFile);
        void parseInputFile(std::ifstream &inputFile);
		int checkDate(std::string dateStr);
        double checkValue(std::string valueStr);
        void matchWithDatabase(int date, double value);

        void run(char *fileName);

    private:
        std::map<int, double>	database;
};