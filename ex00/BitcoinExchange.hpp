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
		void checkDate(std::string date);
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

BitcoinExchange::checkDate(std::string date) {
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
	std::string year = std::to_string(now->tm_year + 1900);
	std::string month = std::to_string(now->tm_mon + 1);
	std::string day = std::to_string(now->tm_mday);
	int dateInt = atoi(date.substr(0, 4) + date.substr(5, 2) + date.substr(8, 2));

	if (atoi(year + month + day) < dateInt) {
		throw std::invalid_argument("Error: future date =>" + date);
	}
	if (date.length() != 10 || date.find_first_not_of("0123456789-") != std::string::npos
		|| (date[4] != '-' || date[7] != '-')) {
		throw std::invalid_argument("Error: bad date format =>" + date);
	}
	if (atoi(date.substr(0, 4)) > (now->tm_year + 1900)) {
		throw std::invalid_argument("Error: bad year =>" + date);
	}
	if (atoi(date.substr(5, 2)) > 12 || atoi(date.substr(5, 2)) < 1) {
		throw std::invalid_argument("Error: bad month =>" + date);
	}
	if (atoi(date.substr(8, 2)) > 31 || atoi(date.substr(8, 2)) < 1) {
		throw std::invalid_argument("Error: bad day =>" + date);
	}
}

BitcoinExchange::parseIn(std::ifstream &inputFile) {
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
