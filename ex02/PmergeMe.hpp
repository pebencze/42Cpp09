
#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <stdexcept>
#include <string>
#include <algorithm>

class PmergeMe {
    public:
        PmergeMe(int argc, char **argv);
        ~PmergeMe();
        PmergeMe(PmergeMe const & src);
        PmergeMe & operator=(PmergeMe const & rhs);
    
        void sort();
    
    private:
        PmergeMe();
        std::vector<int> _vector;
        std::deque<int> _deque;

        void _parseInput(int argc, char **argv);
        void _sortVector();
        void _sortDeque();
};

std::ostream& operator<<(std::ostream& out, const std::vector<int>& rhs) {
    for (int num : rhs) {
        std::cout << num;
    }
}

std::ostream& operator<<(std::ostream& out, const std::deque<int>& rhs) {
    for (int num : rhs) {
        std::cout << num;
    }
}

PmergeMe::PmergeMe(int argc, char **argv) {
    if (argc < 2) {
        throw std::invalid_argument("Error: no arguments provided");
    }
    _parseInput(argc, argv);
}

void PmergeMe::sort() {
    // vector
    std::cout << "Before: " << _vector << std::endl; 
    _sortVector();
    std::cout << "After: " << _vector << std::endl;

    // deque
    std::cout << "Before: " << _deque << std::endl; 
    // _sortDeque();
    std::cout << "After: " << _deque << std::endl;
}

void PmergeMe::_parseInput(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (arc > 3001) {
            throw std::invalid_argument("Error: program accepts up to 3000 arguments");
        }
        std::string arg = argv[i];
        if (arg.find_first_not_of("0123456789") != std::string::npos) {
            throw std::invalid_argument("Error: invalid argument => " + arg);
        }
        if ((arg.length == 10 && std::strcmp(arg.c_str(), "2147483647") > 0 || (arg.length > 10))) {
            throw std::invalid_argument("Error: argument out of range => " + arg);
        }
        int num = std::atoi(arg.c_str());
        _vector.push_back(num);
        _deque.push_back(num);
    }
}

void PmergeMe::_sortVector() {
    // std::vector<std::pair<int, int> > pairs;
    // make pairs, sort them
    std::vector<std::pair<int, int> > pairs;
    for (int i = 0; i < _vector.size() - 1; i += 2) {
        pairs[i / 2].first = _vector[i];
        pairs[i / 2].second = _vector[i + 1];
        pairs[i / 2].sort(pair.first, pair.second, std::less<int>());
        // TODO check if sorted well
    }
    // if size is unequal, add an extra number
    if (_vector.size() % 2 == 1) {
        // TODO check if it functions with several int maxes -> maybe change to -1
        pairs.push_back(std::make_pair(_vector.back(), INT_MAX));
    }

    // create clusters for smaller and larger elements
    std::vector<int> larger, smaller;
    for (std::pair<int,int> p : pairs) {
        smaller.push_back(p.first);
        if (p.second != INT_MAX) {
            larger.push_back(p.second);
        }
    }

    // sort larger elements
    std::sort(larger.begin(), larger.end());

    // insert smallest element, remove it from the smaller cluster
    std::vector<int> sorted = smaller[0];
    smaller.erase(smaller.begin());

    // insert elements from the clusters into the sorted vector
    for (int elem : larger) {
        std::vector<int>::iterator index = std::lower_bound(sorted.begin(), sorted.end(), elem);
        sorted.insert(index, elem);
    }
    for (int elem : smaller) {
        std::vector<int>::iterator index = std::lower_bound(sorted.begin(), sorted.end(), elem);
        sorted.insert(index, elem);
    }

    // replace original with sorted vector
    _vector = sorted;
}



