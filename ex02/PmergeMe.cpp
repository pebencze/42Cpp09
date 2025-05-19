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
    //1. run algorithm for vector
    std::cout << "Before: " << _vector << std::endl;

	clock_t start = std::clock();
    _fordJohnsonVector();
    clock_t end = std::clock();

	std::cout << "After: " << _vector << std::endl;
	long cpuMicroSeconds = end - start;
	std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector: "<< cpuMicroSeconds << " us." << std::endl;

    //2. run algorithm for deque
    std::cout << "Before: " << _deque << std::endl;

	start = std::clock();
    __fordJohnsonDeque();
    end = std::clock();

	std::cout << "After: " << _deque << std::endl;
	cpuMicroSeconds = end - start;
	std::cout << "Time to process a range of " << _deque.size() << " elements with std::vector: "<< cpuMicroSeconds << " us." << std::endl;

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

void PmergeMe::_fordJohnsonVector(std::vector<int>& vec) {
	// DIVIDE INTO PAIRS
	if (vec.size() < 2)
        return ;

    bool leftover = vec.size() % 2;
    if (leftover == 1) {
        vec.push_back(vec.back());
        vec.pop_back();
        leftover = 0;
    }


    std::vector<std::pair<int, int>> pairs;
    for (size_t i = 0; i < vec.size() - 1; i += 2) {
        int a = vec[i] < vec[i + 1] ? vec[i] : vec[i + 1];
        int b = vec[i] < vec[i + 1] ? vec[i + 1] : vec[i];
        pairs.push_back(std::make_pair(a, b));
    }
	// recursion
	
	// CREATE SEQUENCES
	std::vector<int> smaller; // smallest + all larger
	std::vector<int> larger; // all smaller + odd element

	for () {
		
	}

	// INSERT
	if (pend.empty())
		;
	else
	{
		// perform binary search using Jacobsthal numbers
		int insertionsDone = 0;
		for (int n = 3;;n++) {
			int currJacobsthal = _jacobsthalRecursive(n);
			int prevJacobsthal = _jacobsthalRecursive(n - 1);
			int areaOfSearch = currJacobsthal + insertionsDone;
			int NbOfInsertions = currJacobsthal - prevJacobsthal;
			if ((int)pend.size() < NbOfInsertions)
				break; // TODO insert in reverse order
			std::vector<int>::iterator pendIterator = pend.begin() + NbOfInsertions - 1;
			while (insertionsDone < NbOfInsertions) {
				std::vector<int>::iterator index = std::upper_bound(main.begin(), main.begin() + areaOfSearch, *pendIterator);
				main.insert(index, *pendIterator);
				pendIterator = pend.erase(pendIterator);
				pendIterator--;
				insertionsDone++;
				if (index - main.begin() == currJacobsthal + insertionsDone)
					areaOfSearch -= 1;
			}
		}

		// if pend size is smaller than Jacobsthal, use binary search in reverse order
		insertionsDone = 0;
		for (std::vector<int>::reverse_iterator rit = pend.rbegin(); rit != pend.rend(); rit++) {
			// calculate the area of search -> since we are inserting in reverse order we need to shrink the area by the number of insertions already done
			int areaOfSearch = main.size() - insertionsDone - 1 + isOdd; // TODO maybe add plus 1 for odd element
			std::vector<int>::iterator index = std::lower_bound(main.begin(), main.begin() + areaOfSearch, *rit);
			main.insert(index, *rit);
			// pend.erase(rit.base() - 1);
			insertionsDone++;
		}
	}
	_vector = larger;
}

int PmergeMe::_jacobsthalRecursive(int n) {
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return (_jacobsthalRecursive(n - 1) + 2 * _jacobsthalRecursive(n - 2));
}
