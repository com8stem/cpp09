#include "RPN.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./RPN \"[expression]\"" << std::endl;
        return 1;
    }
    try
    {
        RPN calculator(argv[1]);
        calculator.calc_by_RPN();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}