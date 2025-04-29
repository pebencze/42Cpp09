
#pragma once
#include <iostream>
#include <stack>
#include <stdexcept>

class RPN {
    public:
        RPN();
        ~RPN();
        RPN(RPN const & src);
        RPN & operator=(RPN const & src);

        void calculate(char *input);
        void digestInput(char *input);
        void validateInput(char *input);
        void handleOperation(char op);
        void printResult(void);

    private:
        std::stack<int> _stack;
};
