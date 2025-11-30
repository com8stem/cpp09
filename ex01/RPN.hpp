#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <stdexcept>

class RPN
{
    private:
        std::stack<int> container_;
        std::string expression_;
        RPN();
    
    public:
        RPN(char* line);
        RPN(const RPN &);
        RPN &operator=(const RPN &);
        ~RPN();
        void calc_by_RPN();
};

#endif