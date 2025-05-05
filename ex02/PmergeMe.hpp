
#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <ctime>
#include <string.h>

template <typename Container>
class PmergeMe {
    public:
        PmergeMe(int argc, char **argv);
        ~PmergeMe();
        PmergeMe(PmergeMe const & src);
        PmergeMe & operator=(PmergeMe const & rhs);

        void run();

    private:
        PmergeMe();
        std::vector<int> _vector;
        std::deque<int> _deque;

        void _parseInput(int argc, char **argv);
        void _sortVector();
        void _sortDeque();
};

template <typename Container>
std::ostream& operator<<(std::ostream& out, const Container& rhs);
// std::ostream& operator<<(std::ostream& out, const std::deque<int>& rhs);
