#include "RPN.hpp"

RPN::RPN()
{
}

RPN::RPN(char *line)
{
    expression_ = static_cast<std::string>(line);
}

RPN::RPN(const RPN &other) : container_(other.container_), expression_(other.expression_)
{
}

RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
    {
        this->container_ = other.container_;
        this->expression_ = other.expression_;
    }
    return *this;
}

RPN::~RPN()
{
}

bool is_sign(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return true;
    return false;
}

void RPN::calc_by_RPN()
{
    long a;
    long b;
    long result;
    char c;

    for (size_t i = 0; i < expression_.size(); i++)
    {
        c = expression_[i];
        if (isdigit(c))
        {
            if (i + 1 < expression_.size() && isdigit(expression_[i + 1]))
                throw std::runtime_error("Error");
            else
                this->container_.push(c - '0');
        }
        else if (is_sign(c))
            if (this->container_.size() < 2)
                throw std::runtime_error("Error");
            else
            {
                b = this->container_.top();
                this->container_.pop();
                a = this->container_.top();
                this->container_.pop();
                if (c == '+')
                {
                    result = a + b;
                    if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min())
                        throw std::runtime_error("Error: overflow");
                    this->container_.push(result);
                }
                else if (c == '-')
                {
                    result = a - b;
                    if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min())
                        throw std::runtime_error("Error: overflow");
                    this->container_.push(result);
                }
                else if (c == '*')
                {
                    result = a * b;
                    if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min())
                        throw std::runtime_error("Error: overflow");
                    this->container_.push(result);
                }
                else if (c == '/')
                {
                    if (b == 0)
                        throw std::runtime_error("Error: divide by zero");
                    result = a / b;
                    if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min())
                        throw std::runtime_error("Error: overflow");
                    this->container_.push(result);
                }
            }
        else if (isspace(c))
            continue;
        else
            throw std::runtime_error("Error");
    }
    if (this->container_.size() != 1)
        throw std::runtime_error("Error: invalid expression");
    std::cout << this->container_.top() << std::endl;
}