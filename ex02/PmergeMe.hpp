
#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <ctime>
#include <string.h>

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
        std::vector<int> _fordJohnsonVector(std::vector<int>& vec);
        std::deque<int> _fordJohnsonDeque(std::deque<int>& deq);
		int _jacobsthalRecursive(int n);
	};

// template <typename Container>
std::ostream& operator<<(std::ostream& out, const std::vector<int>& rhs);
std::ostream& operator<<(std::ostream& out, const std::deque<int>& rhs);
