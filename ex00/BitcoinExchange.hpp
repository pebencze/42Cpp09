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

        void run(char *fileName);
        
    private:
        std::map<int, double>	_database;

        void _parseCsvFile(std::ifstream &csvFile);
        void _parseInputFile(std::ifstream &inputFile);
		int _checkDate(std::string dateStr);
        double _checkValue(std::string valueStr);
        void _matchWithDatabase(int date, double value);
};