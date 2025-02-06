#pragma once
#include <iostream>
#include <map>

class BitcoinExchange {
    public:
		BitcoinExchange();
		~BitcoinExchange();
		BitcoinExchange(BitcoinExchange const & src);
		BitcoinExchange & operator=(BitcoinExchange const & src);

        parseMap(std::ifstream &inputFile);
        void calculate();

    private:
        std::map<std::string, double>	database;
		std::map<std::string, double>	input;
};

BitcoinExchange::BitcoinExchange() {

}

BitcoinExchange::~BitcoinExchange() {

}

BitcoinExchange::BitcoinExchange(BitcoinExchange const & src) {
	*this = src;
}

BitcoinExchange & BitcoinExchange::operator=(BitcoinExchange const & src) {
	if (this != &src) {
	}
	return *this;
}


BitcoinExchange::parseMap(std::ifstream &inputFile) {
    std::string date;
	std::string line;

	getline(inputFile, line);
	if (line != "date | value") {
		std::cerr << "Error: missing header\n";
		return ;
	}
    while (getline(inputFile, line)) {
		std::stringstream ss(line);
		std::string date, valueStr;
		double value;

		try {
			if (getline(ss, date, '|') && getline(ss, valueStr)) {
				date.erase(remove_if(date.begin(), date.end(), isspace()));
				value.erase(remove_if(value.begin(), value.end(), isspace()));
				checkDate(date);
				errno = 0;
				value = std::strtod(valueStr.c_str(), NULL);
				if (errno = ERANGE || value < 0 || value > 1000) {
					throw std::out_of_range("Error: value out of range 0 - 1000 =>" + valueStr);
				}
    			input.emplace(date, value);
			} else {
				 throw std::invalid_argument("Error: bad input =>" + line);
			}
		} catch (std::exception & e) {
			std::cerr << e.what() << std::endl;
		}
    }

}
