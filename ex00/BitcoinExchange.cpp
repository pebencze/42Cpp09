
#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {

}

BitcoinExchange::~BitcoinExchange() {

}

BitcoinExchange::BitcoinExchange(BitcoinExchange const & src) {
	*this = src;
}

BitcoinExchange & BitcoinExchange::operator=(BitcoinExchange const & src) {
	if (this != &src) {
        this->database = src.database;
	}
	return *this;
}

void BitcoinExchange::run(char *fileName){
    std::ifstream	inputFile(fileName);
    std::ifstream	csvFile(CSV_FILE);
    std::string		line;

    if (!inputFile.is_open() || !csvFile.is_open()) {
        try {
            inputFile.close();
            csvFile.close();
        } catch (std::ios_base::failure) {}
        throw std::runtime_error("Error: could not open a file.");
    }

    this->parseCsvFile(csvFile);
	this->parseInputFile(inputFile);

    inputFile.close();
    csvFile.close();
}

void BitcoinExchange::parseCsvFile(std::ifstream &csvFile) {
    std::string line;
    std::string date;
    std::string value;

    while (std::getline(csvFile, line)) {
        std::stringstream ss(line);
        if (line == "date,exchange_rate")
            continue;
        if (std::getline(ss, date, ',') && std::getline(ss, value)) {
            date.erase(std::remove_if(date.begin(), date.end(), static_cast<int(*)(int)>(std::isspace)), date.end());
            value.erase(std::remove_if(value.begin(), value.end(), static_cast<int(*)(int)>(std::isspace)), value.end());
            int dateInt = std::atoi((date.substr(0, 4) + date.substr(5, 2) + date.substr(8, 2)).c_str());
            database.insert(std::make_pair(dateInt, std::strtod(value.c_str(), NULL)));
        }
    }
}

int BitcoinExchange::checkDate(std::string dateStr) {
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
	int year = now->tm_year + 1900;
	int month = now->tm_mon + 1;
	int day = now->tm_mday;
    int currentDate = year * 10000 + month * 100 + day;

    dateStr.erase(std::remove_if(dateStr.begin(), dateStr.end(), static_cast<int(*)(int)>(std::isspace)), dateStr.end());
	int date = std::atoi((dateStr.substr(0, 4) + dateStr.substr(5, 2) + dateStr.substr(8, 2)).c_str());

	if (currentDate < date) {
		throw std::invalid_argument("Error: future date => " + dateStr);
	}
	if (dateStr.length() != 10 || dateStr.find_first_not_of("0123456789-") != std::string::npos
		|| (dateStr[4] != '-' || dateStr[7] != '-')) {
		throw std::invalid_argument("Error: bad date format => " + dateStr);
	}
	if (std::atoi(dateStr.substr(0, 4).c_str()) > (now->tm_year + 1900)) {
		throw std::invalid_argument("Error: bad year => " + dateStr);
	}
	if (std::atoi(dateStr.substr(5, 2).c_str()) > 12 || std::atoi(dateStr.substr(5, 2).c_str()) < 1) {
		throw std::invalid_argument("Error: bad month => " + dateStr);
	}
	if (std::atoi(dateStr.substr(8, 2).c_str()) > 31 || std::atoi(dateStr.substr(8, 2).c_str()) < 1) {
		throw std::invalid_argument("Error: bad day => " + dateStr);
	}

    return date;
}

double BitcoinExchange::checkValue(std::string valueStr) {
    double value;
    errno = 0;
    char *endptr;

    valueStr.erase(std::remove_if(valueStr.begin(), valueStr.end(), static_cast<int(*)(int)>(std::isspace)), valueStr.end());
	value = std::strtod(valueStr.c_str(), &endptr);
    if (*endptr != '\0' || valueStr.empty()) {
        throw std::invalid_argument("Error: bad input => " + valueStr);
    }
	if (errno == ERANGE || value < 0 || value > 1000) {
		throw std::invalid_argument("Error: value out of range 0 - 1000");
	}

    return value;
}

void BitcoinExchange::parseInputFile(std::ifstream &inputFile) {
    std::string date;
	std::string line;

	std::getline(inputFile, line);
    line.erase(std::remove_if(line.begin(), line.end(), static_cast<int(*)(int)>(std::isspace)), line.end());
	if (line != "date|value")
        throw std::runtime_error("Error: missing header");

	while (std::getline(inputFile, line)) {
		std::stringstream ss(line);
		std::string dateStr, valueStr;
		try {
			if (std::getline(ss, dateStr, '|') && std::getline(ss, valueStr)) {
				int date = checkDate(dateStr);
                double value = checkValue(valueStr);
                matchWithDatabase(date, value);
			} else {
				 throw std::invalid_argument("Error: bad input => " + line);
			}
		} catch (std::invalid_argument & e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void BitcoinExchange::matchWithDatabase(int date, double value) {
    std::map<int, double>::iterator it = database.lower_bound(date);
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;

    std::ostringstream formattedDate;
    formattedDate << year << "-" 
        << (month < 10 ? "0" : "") << month << "-"
        << (day < 10 ? "0" : "") << day;
    

    if (it == database.end())
        it--;
    if (it == database.begin())
        throw std::invalid_argument("Error: no lower bound for date => " + formattedDate.str());
    
    
    std::cout << formattedDate.str()
        << " => " << value << " = " << value * it->second << std::endl;
}
