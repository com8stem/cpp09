#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>

class PmergeMe
{
private:
    std::vector<int> vec_;
    std::list<int> list_;
    PmergeMe();

public:
    PmergeMe(int argc, char **argv);
    PmergeMe(const PmergeMe &);
    PmergeMe &operator=(const PmergeMe &);
    ~PmergeMe();

    void MIsort_vec();
    void MIsort_list();
    int Jacobsthal(int k);
};

#endif