
#include "PmergeMe.hpp"

std::ostream& operator<<(std::ostream& out, const std::vector<int>& rhs) {
	std::vector<int>::const_iterator it;
	for (it = rhs.begin(); it != rhs.end(); it++) {
		out << *it << " ";
	}
	return out;
}

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(int argc, char **argv) {
	if (argc < 2) {
		throw std::invalid_argument("Error: no arguments provided");
	}
	_parseInput(argc, argv);
}

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(PmergeMe const & src) {
	*this = src;
}

PmergeMe & PmergeMe::operator=(PmergeMe const & rhs) {
	if (this != &rhs) {
		this->_deque = rhs._deque;
		this->_vector = rhs._vector;
	}
	return *this;
}

void PmergeMe::run() {
    // vector
    std::cout << "Before: " << _vector << std::endl;
	clock_t start = std::clock();
    _sortVector();

	clock_t end = std::clock();
	std::cout << "After: " << _vector << std::endl;
	long cpuMicroSeconds = end - start;
	std::cout << "Time to process a range of 3000 elements with std::[...]: "<< cpuMicroSeconds << " us." << std::endl;

    // deque
	// start = std::clock();
    //std::cout << "Before: " << _deque << std::endl;
    // _sortDeque();
	//
	// end = std::clock();
    //std::cout << "After: " << _deque << std::endl;
	// cpuMicroSeconds = end - start;
	// std::cout << cpuMicroSeconds << std::endl;
}

void PmergeMe::_parseInput(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (argc > 3001) {
            throw std::invalid_argument("Error: program accepts up to 3000 arguments");
        }
        std::string arg = argv[i];
        if (arg.find_first_not_of("0123456789") != std::string::npos) {
            throw std::invalid_argument("Error: invalid argument => " + arg);
        }
        if ((arg.length() == 10 && strcmp(arg.c_str(), "2147483647") > 0) || (arg.length() > 10)) {
            throw std::invalid_argument("Error: argument out of range => " + arg);
        }
        int num = std::atoi(arg.c_str());
        _vector.push_back(num);
        _deque.push_back(num);
    }
}

// void PmergeMe::_sortVector() {
//     // make pairs, sort them
//     std::vector<std::pair<int, int> > pairs;
//     for (size_t i = 0; i < (_vector.size() - 1); i += 2) {
//         std::pair<int,int> newPair = std::make_pair(std::min(_vector[i], _vector[i + 1]), std::max(_vector[i], _vector[i + 1]));
//         pairs.push_back(newPair);
//     }
//     // if size is unequal, add an extra number
//     if (_vector.size() % 2 == 1) {
//         // TODO check if it functions with several int maxes -> maybe change to -1
//         pairs.push_back(std::make_pair(_vector.back(), -1));
//     }

//     // create clusters for smaller and larger elements
//     std::vector<int> larger, smaller;
//     for (size_t i = 0; i < pairs.size(); i++) {
//         std::pair<int, int> p = pairs[i];
//         smaller.push_back(p.first);
//         if (p.second != -1) {
//             larger.push_back(p.second);
//         }
//     }

//     // // sort larger elements
//     std::sort(larger.begin(), larger.end());

//     // insert smallest element, remove it from the smaller cluster
//     std::vector<int> sorted;
//     sorted.push_back(smaller[0]);
//     smaller.erase(smaller.begin());

//     // insert elements from the clusters into the sorted vector
//     for (size_t i = 0; i < larger.size(); i++) {
//         int elem = larger[i];
//         std::vector<int>::iterator index = std::lower_bound(sorted.begin(), sorted.end(), elem);
//         sorted.insert(index, elem);
//     }
//     for (size_t i = 0; i < smaller.size(); i++) {
//         int elem = smaller[i];
//         std::vector<int>::iterator index = std::lower_bound(sorted.begin(), sorted.end(), elem);
//         sorted.insert(index, elem);
//     }

//     // replace original with sorted vector
//     _vector = sorted;
// }

void PmergeMe::_swapPairs(int distance, std::vector<int>::iterator offset) {
	// swap pairs
	for (int i = 0; i < distance; i++) {
		std::swap(*(offset + i), *(offset + distance + i));
	}
}

void PmergeMe::_sortVector() {
	static int unitSize = 1;
	// dividing into pairs (merge)
	int unitCount = _vector.size() / unitSize;
	if (unitSize > (int)_vector.size() / 2) // base case
		return ;

	bool isOdd = unitCount % 2;

	std::vector<int>::iterator start = _vector.begin();
	int sizeWithoutLeftover = (unitCount * unitSize) - (isOdd * unitSize);
	std::vector<int>::iterator end = _vector.begin() + sizeWithoutLeftover;

	for (std::vector<int>::iterator it = start; it != end; it += (unitSize * 2)) {
		// sort pairs
		int rightMostOfPair1 = unitSize - 1;
		int rightMostOfPair2 = unitSize * 2 - 1;
		if (*(it + rightMostOfPair1) > *(it + rightMostOfPair2))
			_swapPairs(unitSize, it);
	}
	unitSize *= 2;
	// recursion
	// _sortVector();
	unitSize /= 2;

	// create sequences
	std::vector<int> main; // smallest + all larger
	std::vector<int> pend; // all smaller + odd element

	start = _vector.begin();
	end = _vector.end();

	int i = 0;
	for (std::vector<int>::iterator it = start + unitSize; it != end; it += unitSize) {
		if (it == start || i % 2 == 1)
			_pushBackRange(it, main, unitSize);
		else
			_pushBackRange(it, pend, unitSize);
		i++;
	}

	std::cout << "main: " << main << std::endl;
	std::cout << "pend: " << pend << std::endl;
	// insert using Jacobsthal numbers;
}

void PmergeMe::_pushBackRange(std::vector<int>::iterator start, std::vector<int>& vec, int unitSize) {
	std::vector<int>::iterator end = start + unitSize;
	for (std::vector<int>::iterator it = start; it != end; it++) {
		vec.push_back(*it);
	}
}
