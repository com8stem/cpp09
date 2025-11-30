#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./PmergeMe [numbers]" << std::endl;
        return 1;
    }

    PmergeMe pmergeme(argc, argv);
    pmergeme.MIsort_vec();
    pmergeme.MIsort_list();
    return 0;
}