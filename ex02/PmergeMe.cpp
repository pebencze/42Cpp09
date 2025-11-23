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
    _fordJohnsonVector(_vector);
    clock_t end = std::clock();

	std::cout << "After: " << _vector << std::endl;
	long cpuMicroSeconds = end - start;
	std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector: "<< cpuMicroSeconds << " us." << std::endl;
	assert(std::is_sorted(_vector.begin(), _vector.end()));

    // //2. run algorithm for deque
    // std::cout << "Before: " << _deque << std::endl;

	// start = std::clock();
    // _deque = _fordJohnsonDeque(_deque);
    // end = std::clock();

	// std::cout << "After: " << _deque << std::endl;
	// cpuMicroSeconds = end - start;
	// std::cout << "Time to process a range of " << _deque.size() << " elements with std::vector: "<< cpuMicroSeconds << " us." << std::endl;
	// assert(std::is_sorted(_deque.begin(), _deque.end()));
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

/* creates Jacobsthal order */
static std::vector<uint64_t> buildJacobsthalUpTo(size_t limit) {
    std::vector<uint64_t> jacobsthal;
    jacobsthal.push_back(0);
    if (limit == 0) return jacobsthal;
    jacobsthal.push_back(1);
    while (jacobsthal.back() < limit) {
        size_t s = jacobsthal.size();
        uint64_t next = jacobsthal[s-1] + 2 * jacobsthal[s-2];
        jacobsthal.push_back(next);
        if (jacobsthal.size() > 64) break;
    }
    return jacobsthal;
}

/* generate Jacobsthal order for size of B vector plus 2 */
static std::vector<size_t> generateInsertionOrderJacobsthal(size_t m) {
    std::vector<size_t> order;
    if (m == 0) return order;
    std::vector<uint64_t> J = buildJacobsthalUpTo(m+2);
    // build blocks by Jacobsthal boundaries
    int k = (int)J.size() - 1;
    while (k > 0 && J[k] >= m) --k;
    for (int r = k+1; r >= 1; --r) {
        size_t L = (size_t)J[r-1];
        size_t R = (size_t)std::min<uint64_t>(J[r], m);
        for (size_t i = L; i < R; ++i) order.push_back(i);
    }
    // deduplicate and append missing
    std::vector<char> seen(m, 0);
    std::vector<size_t> uniq;
    for (std::vector<size_t>::const_iterator it = order.begin(); it != order.end(); ++it) {
        size_t x = *it;
        if (x < m && !seen[x]) { 
            uniq.push_back(x); 
            seen[x]=1; 
        }
    };
    for (size_t i = 0; i < m; ++i) if (!seen[i]) uniq.push_back(i);
    return uniq;
}

/* binary insert into vector V */
void binaryInsert(std::vector<int>& V, const int& value) {
    size_t low = 0, high = V.size();
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (value < V[mid]) high = mid;
        else low = mid + 1;
    }
    V.insert(V.begin() + low, value);
}

/* main algorithm for vector */
void PmergeMe::_fordJohnsonVector(std::vector<int>& arr) {
    size_t n = arr.size();
    if (n <= 1) return;
    if (n <= 8) { // small sort
        std::sort(arr.begin(), arr.end());
        return;
    }

    // 1) pair and order each pair so b_i <= a_i; collect pairs
    size_t pairs = n / 2;
    bool has_unpaired = (n % 2 == 1);
    std::vector<int> a_list; a_list.reserve(pairs); // a_i (larger of each pair)
    std::vector<int> b_list; b_list.reserve(pairs); // b_i (smaller of each pair)
    for (size_t i = 0; i < pairs; ++i) {
        int &x = arr[2*i], &y = arr[2*i + 1];
        if (x < y) { 
            b_list.push_back(x); 
            a_list.push_back(y); 
        }
        else       { 
            b_list.push_back(y); 
            a_list.push_back(x); 
        }
    }

    int unpaired_val; bool has_u = false;
    if (has_unpaired) { has_u = true; unpaired_val = arr.back(); }

    // 2) build main and secondary arrays
    // main = [b1, a1, a2, ..., ap]  (b1 + a1...an into main chain)
    // secondary = [b2, b3, ..., bp] (remaining smalls)
    std::vector<int> main; main.reserve(a_list.size() + 1);
    std::vector<int> secondary; secondary.reserve(b_list.size());
    if (!b_list.empty()) {
        // promote b1
        main.push_back(b_list[0]);
        // push all a_i into main
        for (size_t i = 0; i < a_list.size(); ++i) main.push_back(a_list[i]);
        // push remaining b's to secondary starting from index 1
        for (size_t i = 1; i < b_list.size(); ++i) secondary.push_back(b_list[i]);
    } else {
        // No pairs? shouldn't happen for n>=2, but handle gracefully
        for (size_t i = 0; i < a_list.size(); ++i) main.push_back(a_list[i]);
    }

    // 3) recursively sort main chain using same algorithm
    _fordJohnsonVector(main); // sorted main chain

    // 4) insert elements of secondary into main using binary insertion in a given order
    // insertion order for secondary is Jacobsthal-based
    std::vector<size_t> order = generateInsertionOrderJacobsthal(secondary.size());
                                    
    for (std::vector<size_t>::const_iterator it = order.begin(); it != order.end(); ++it) {
        size_t idx = *it;
        binaryInsert(main, secondary[idx]);
    }

    // 5) if there was an unpaired u, insert it too
    if (has_u) binaryInsert(main, unpaired_val);

    // 6) place sorted main back into arr
    arr = std::move(main);
}