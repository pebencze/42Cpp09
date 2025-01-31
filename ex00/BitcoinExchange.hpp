#pragma once
#include <iostream>
#include <map>

class BitcoinExchange {
    public: 
        parseMap(std::string line);
        void calculate(std::map<struct tm, int> const &map);

    private:
        std::map<struct tm, int>    data;
};

BitcoinExchange::parseMap(std::string line, std::map<int, int> map) {
    int num;
    std::string date;

    if (line.size() < 14)
        throw   std::runtime_error("Error: bad input");    
    date = line.substr(0, 4) + line.substr(5, 2) + line.substr(7, 2);
    num = atoi(date.c_str());
    map.insert(std::pair{, });
    map[];
}