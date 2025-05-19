#include "PmergeMe.hpp"

std::ostream& operator<<(std::ostream& out, const std::vector<int>& rhs) {
	std::vector<int>::const_iterator it;
	for (it = rhs.begin(); it != rhs.end(); it++) {
		out << *it << " ";
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, const std::deque<int>& rhs) {
	std::deque<int>::const_iterator it;
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
    _vector = _fordJohnsonVector(_vector);
    clock_t end = std::clock();

	std::cout << "After: " << _vector << std::endl;
	long cpuMicroSeconds = end - start;
	std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector: "<< cpuMicroSeconds << " us." << std::endl;

    //2. run algorithm for deque
    std::cout << "Before: " << _deque << std::endl;

	start = std::clock();
    _deque = _fordJohnsonDeque(_deque);
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

std::vector<int> PmergeMe::_fordJohnsonVector(std::vector<int>& vec) {
	// DIVIDE INTO PAIRS --------------------------------------------
	if (vec.size() < 2)
        return vec;

    // check if the vector is odd, store leftover
    bool hasLeftover = vec.size() % 2;
    int leftover;
    if (hasLeftover == true) {
        leftover = vec.back();
        vec.pop_back();
    }

    // create pairs
    std::vector<std::pair<int, int> > pairs;
    for (size_t i = 0; i < vec.size() - 1; i += 2) {
        int a = vec[i] < vec[i + 1] ? vec[i] : vec[i + 1];
        int b = vec[i] < vec[i + 1] ? vec[i + 1] : vec[i];
        pairs.push_back(std::make_pair(a, b));
    }
	
	// CREATE SEQUENCES ----------------------------------------------
	std::vector<int> smaller; // all smaller + odd element
	std::vector<int> larger; // smallest + all larger

	for (int i = 0; i < (int)pairs.size(); i++) {
        if (i == 0) {
            smaller.push_back(pairs[i].first);
            larger.push_back(pairs[i].second);
        } else {
            smaller.push_back(pairs[i].first);
            larger.push_back(pairs[i].second);
        }
    }
    // recursion
    larger = _fordJohnsonVector(larger);

	// INSERT ---------------------------------------------------------
	if (smaller.empty())
		return larger;
	// perform binary search using Jacobsthal numbers
	int insertionsDone = 0;
	for (int n = 3;;n++) {
		int currJacobsthal = _jacobsthalRecursive(n);
		int prevJacobsthal = _jacobsthalRecursive(n - 1);
		int areaOfSearch = currJacobsthal + insertionsDone;
		int NbOfInsertions = currJacobsthal - prevJacobsthal;
		if ((int)smaller.size() < NbOfInsertions)
			break; // TODO insert in reverse order
        int indexInSmaller = NbOfInsertions - 1;
		while (insertionsDone < NbOfInsertions) {
			std::vector<int>::iterator newIndex = std::upper_bound(larger.begin(), larger.begin() + areaOfSearch, smaller[indexInSmaller]);
			larger.insert(newIndex, smaller[indexInSmaller]);
            smaller.erase(smaller.begin() + indexInSmaller);
            indexInSmaller--;
			insertionsDone++;
			if (newIndex - larger.begin() == currJacobsthal + insertionsDone)
				areaOfSearch -= 1;
		}
	}
	// if pend size is smaller than Jacobsthal, use binary search in reverse order
	insertionsDone = 0;
	for (std::vector<int>::reverse_iterator rit = smaller.rbegin(); rit != smaller.rend(); rit++) {
		// calculate the area of search -> since we are inserting in reverse order we need to shrink the area by the number of insertions already done
		int areaOfSearch = larger.size() - insertionsDone - 1; // + (hasLeftover ? 1 : 0);
		std::vector<int>::iterator index = std::lower_bound(larger.begin(), larger.begin() + areaOfSearch, *rit);
		larger.insert(index, *rit);
		insertionsDone++;
	}

    // if there is a leftover, insert it in the right place
    if (hasLeftover == true) {
        std::vector<int>::iterator index = std::lower_bound(larger.begin(), larger.end(), leftover);
        larger.insert(index, leftover);
    }

	return larger;
}


std::deque<int> PmergeMe::_fordJohnsonDeque(std::deque<int>& deq) {
	// DIVIDE INTO PAIRS --------------------------------------------
	if (deq.size() < 2)
        return deq;

    // check if the vector is odd, store leftover
    bool hasLeftover = deq.size() % 2;
    int leftover;
    if (hasLeftover == true) {
        leftover = deq.back();
        deq.pop_back();
    }

    // create pairs
    std::deque<std::pair<int, int> > pairs;
    for (size_t i = 0; i < deq.size() - 1; i += 2) {
        int a = deq[i] < deq[i + 1] ? deq[i] : deq[i + 1];
        int b = deq[i] < deq[i + 1] ? deq[i + 1] : deq[i];
        pairs.push_back(std::make_pair(a, b));
    }
	
	// CREATE SEQUENCES ----------------------------------------------
	std::deque<int> smaller; // all smaller + odd element
	std::deque<int> larger; // smallest + all larger

	for (int i = 0; i < (int)pairs.size(); i++) {
        if (i == 0) {
            smaller.push_back(pairs[i].first);
            larger.push_back(pairs[i].second);
            continue;
        }
		smaller.push_back(pairs[i].first);
		larger.push_back(pairs[i].second);
	}

    // recursion
    larger = _fordJohnsonDeque(larger);

	// INSERT ---------------------------------------------------------
	if (smaller.empty())
		return larger;
	// perform binary search using Jacobsthal numbers
	int insertionsDone = 0;
	for (int n = 3;;n++) {
		int currJacobsthal = _jacobsthalRecursive(n);
		int prevJacobsthal = _jacobsthalRecursive(n - 1);
		int areaOfSearch = currJacobsthal + insertionsDone;
		int NbOfInsertions = currJacobsthal - prevJacobsthal;
		if ((int)smaller.size() < NbOfInsertions)
			break; // TODO insert in reverse order
        int indexInSmaller = NbOfInsertions - 1;
		while (insertionsDone < NbOfInsertions) {
			std::deque<int>::iterator newIndex = std::upper_bound(larger.begin(), larger.begin() + areaOfSearch, smaller[indexInSmaller]);
			larger.insert(newIndex, smaller[indexInSmaller]);
            smaller.erase(smaller.begin() + indexInSmaller);
            indexInSmaller--;
			insertionsDone++;
			if (newIndex - larger.begin() == currJacobsthal + insertionsDone)
				areaOfSearch -= 1;
		}
	}
	// if pend size is smaller than Jacobsthal, use binary search in reverse order
	insertionsDone = 0;
	for (std::deque<int>::reverse_iterator rit = smaller.rbegin(); rit != smaller.rend(); rit++) {
		// calculate the area of search -> since we are inserting in reverse order we need to shrink the area by the number of insertions already done
		int areaOfSearch = larger.size() - insertionsDone - 1; // + (hasLeftover ? 1 : 0);
		std::deque<int>::iterator index = std::lower_bound(larger.begin(), larger.begin() + areaOfSearch, *rit);
		larger.insert(index, *rit);
		insertionsDone++;
	}

    // if there is a leftover, insert it in the right place
    if (hasLeftover == true) {
        std::deque<int>::iterator index = std::lower_bound(larger.begin(), larger.end(), leftover);
        larger.insert(index, leftover);
    }

	return larger;
}

int PmergeMe::_jacobsthalRecursive(int n) {
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return (_jacobsthalRecursive(n - 1) + 2 * _jacobsthalRecursive(n - 2));
}
