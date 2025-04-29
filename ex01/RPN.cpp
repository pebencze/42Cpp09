
#include "RPN.hpp"

RPN::RPN() {}

RPN::~RPN() {}

RPN::RPN(RPN const & src) {
    _stack = src._stack;
}

RPN & RPN::operator=(RPN const & src) {
    if (this != &src) {
        _stack = src._stack;
    }
    return *this;
}

void RPN::calculate(char *input) {
    validateInput(input);
    digestInput(input);
    printResult();
}

void RPN::validateInput(char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if ((i % 2 == 0 && (!isdigit(input[i]) && input[i] != '+' && input[i] != '-' && input[i] != '*' && input[i] != '/'))
            || (i % 2 == 1 && input[i] != ' ')) {
            throw std::invalid_argument("Error: invalid character in expression");
        }
    }
    if (strlen(input) == 0)
        throw std::invalid_argument("Error: empty expression");
}

void RPN::digestInput(char *input) {
    char *token = input;
    while (*token) {
        if (isdigit(*token))
            _stack.push(std::atoi(token));
        if (*token == '+' || *token == '-' || *token == '*' || *token == '/')
            handleOperation(*token);
        token++;
    }
    if (_stack.size() > 1)
        throw std::invalid_argument("Error: too many operands");
}

void RPN::handleOperation(char op) {
    if (_stack.size() < 2)
        throw std::invalid_argument("Error: not enough operands");
    int b = _stack.top();
    _stack.pop();
    int a = _stack.top();
    _stack.pop();
    if (op == '+')
        _stack.push(a + b);
    else if (op == '-')
        _stack.push(a - b);
    else if (op == '*')
        _stack.push(a * b);
    else if (op == '/') {
        if (b == 0)
            throw std::invalid_argument("Error: division by zero");
        _stack.push(a / b);
    }
}

void RPN::printResult(void) {
    std::cout << _stack.top() << std::endl;
    _stack.pop();
}