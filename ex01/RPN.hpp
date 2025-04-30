
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
        
    private:
        std::stack<int> _stack;

        void _digestInput(char *input);
        void _validateInput(char *input);
        void _handleOperation(char op);
        void _printResult(void);
};
