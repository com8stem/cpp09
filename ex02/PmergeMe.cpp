#include "PmergeMe.hpp"
#include <algorithm>

size_t sort_count = 0;

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(int argc, char **argv)
{
    int i = 1;
    for (; i < argc; i++)
    {
        long num = atol(argv[i]);
        if (num < 0 || num > std::numeric_limits<int>::max())
        {
            std::cerr << "Error: number is out of range" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        this->vec_.push_back(static_cast<int>(num));
        this->list_.push_back(static_cast<int>(num));
    }
}

PmergeMe::PmergeMe(const PmergeMe &other) : vec_(other.vec_), list_(other.list_)
{
}

PmergeMe::~PmergeMe()
{
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        this->vec_ = other.vec_;
        this->list_ = other.list_;
    }
    return *this;
}

int get_jacobsthal(int k)
{
    if (k == 0)
        return 0;
    if (k == 1)
        return 1;
    return get_jacobsthal(k - 1) + 2 * get_jacobsthal(k - 2);
}

std::vector<size_t> sort_indices_vec(const std::vector<int> &data, std::vector<size_t> indices)
{
    if (indices.size() <= 1)
        return indices;

    std::vector<size_t> larger_indices;
    std::vector<size_t> pend_indices;
    size_t straggler_idx = 0;
    bool has_straggler = false;

    if (indices.size() % 2 != 0)
    {
        straggler_idx = indices.back();
        has_straggler = true;
        indices.pop_back();
    }

    for (size_t i = 0; i < indices.size(); i += 2)
    {
        size_t idx1 = indices[i];
        size_t idx2 = indices[i + 1];
        sort_count++;
        if (data[idx1] > data[idx2])
        {
            larger_indices.push_back(idx1);
            pend_indices.push_back(idx2);
        }
        else
        {
            larger_indices.push_back(idx2);
            pend_indices.push_back(idx1);
        }
    }

    std::vector<size_t> sorted_larger = sort_indices_vec(data, larger_indices);

    std::vector<size_t> sorted_pend;
    for (size_t i = 0; i < sorted_larger.size(); i++)
    {
        for (size_t j = 0; j < larger_indices.size(); j++)
        {
            if (larger_indices[j] == sorted_larger[i])
            {
                sorted_pend.push_back(pend_indices[j]);
                break;
            }
        }
    }

    std::vector<size_t> result = sorted_larger;

    result.insert(result.begin(), sorted_pend[0]);

    std::vector<size_t> chain_positions(sorted_larger.size());
    for (size_t k = 0; k < sorted_larger.size(); k++)
    {
        chain_positions[k] = k + 1;
    }

    int jacob_idx = 3;
    size_t prev_jacob = 1;
    size_t inserted_count = 1;

    while (inserted_count < sorted_pend.size())
    {
        size_t curr_jacob = get_jacobsthal(jacob_idx);
        size_t range_end = curr_jacob;
        if (range_end > sorted_pend.size())
            range_end = sorted_pend.size();

        std::vector<size_t> group_upper_bounds(chain_positions);

        for (size_t i = range_end; i > prev_jacob; i--)
        {
            size_t pend_idx = i - 1;
            size_t idx_to_insert = sorted_pend[pend_idx];
            int val_to_insert = data[idx_to_insert];

            size_t upper_bound = group_upper_bounds[pend_idx];

            size_t left = 0;
            size_t right = upper_bound;

            while (left < right)
            {
                size_t mid = left + (right - left) / 2;
                sort_count++;
                if (data[result[mid]] < val_to_insert)
                    left = mid + 1;
                else
                    right = mid;
            }

            result.insert(result.begin() + left, idx_to_insert);

            for (size_t k = 0; k < chain_positions.size(); k++)
            {
                if (chain_positions[k] >= left)
                    chain_positions[k]++;
            }
            inserted_count++;
        }
        prev_jacob = curr_jacob;
        jacob_idx++;
    }

    if (has_straggler)
    {
        int val_to_insert = data[straggler_idx];
        size_t left = 0;
        size_t right = result.size();
        while (left < right)
        {
            size_t mid = left + (right - left) / 2;
            sort_count++;
            if (data[result[mid]] < val_to_insert)
                left = mid + 1;
            else
                right = mid;
        }
        result.insert(result.begin() + left, straggler_idx);
    }

    return result;
}

std::vector<int> merge_insertion_sort_vec(std::vector<int> vec)
{
    if (vec.size() <= 1)
        return vec;

    std::vector<size_t> indices(vec.size());
    for (size_t i = 0; i < vec.size(); i++)
        indices[i] = i;

    std::vector<size_t> sorted_indices = sort_indices_vec(vec, indices);

    std::vector<int> result;
    for (size_t i = 0; i < sorted_indices.size(); i++)
        result.push_back(vec[sorted_indices[i]]);

    return result;
}

std::list<size_t> sort_indices_list(const std::list<int> &data, std::list<size_t> indices)
{
    if (indices.size() <= 1)
        return indices;

    std::list<size_t> larger_indices;
    std::list<size_t> pend_indices;
    size_t straggler_idx = 0;
    bool has_straggler = false;

    if (indices.size() % 2 != 0)
    {
        straggler_idx = indices.back();
        has_straggler = true;
        indices.pop_back();
    }

    std::list<size_t>::iterator it = indices.begin();
    while (it != indices.end())
    {
        size_t idx1 = *it++;
        size_t idx2 = *it++;

        std::list<int>::const_iterator it1 = data.begin();
        std::advance(it1, idx1);
        std::list<int>::const_iterator it2 = data.begin();
        std::advance(it2, idx2);

        sort_count++;
        if (*it1 > *it2)
        {
            larger_indices.push_back(idx1);
            pend_indices.push_back(idx2);
        }
        else
        {
            larger_indices.push_back(idx2);
            pend_indices.push_back(idx1);
        }
    }

    std::list<size_t> sorted_larger = sort_indices_list(data, larger_indices);

    std::list<size_t> sorted_pend;
    for (std::list<size_t>::iterator sit = sorted_larger.begin(); sit != sorted_larger.end(); ++sit)
    {
        std::list<size_t>::iterator lit = larger_indices.begin();
        std::list<size_t>::iterator pit = pend_indices.begin();
        for (; lit != larger_indices.end(); ++lit, ++pit)
        {
            if (*lit == *sit)
            {
                sorted_pend.push_back(*pit);
                break;
            }
        }
    }

    std::list<size_t> result = sorted_larger;

    result.insert(result.begin(), sorted_pend.front());

    std::list<size_t> chain_positions;
    for (size_t k = 0; k < sorted_larger.size(); k++)
    {
        chain_positions.push_back(k + 1);
    }

    int jacob_idx = 3;
    size_t prev_jacob = 1;
    size_t inserted_count = 1;

    while (inserted_count < sorted_pend.size())
    {
        size_t curr_jacob = get_jacobsthal(jacob_idx);
        size_t range_end = curr_jacob;
        if (range_end > sorted_pend.size())
            range_end = sorted_pend.size();

        std::list<size_t> group_upper_bounds = chain_positions;

        for (size_t i = range_end; i > prev_jacob; i--)
        {
            size_t pend_idx = i - 1;

            std::list<size_t>::iterator sp_it = sorted_pend.begin();
            std::advance(sp_it, pend_idx);
            size_t idx_to_insert = *sp_it;

            std::list<int>::const_iterator val_it = data.begin();
            std::advance(val_it, idx_to_insert);
            int val_to_insert = *val_it;

            std::list<size_t>::iterator cp_it = group_upper_bounds.begin();
            std::advance(cp_it, pend_idx);
            size_t upper_bound = *cp_it;

            size_t left = 0;
            size_t right = upper_bound;

            while (left < right)
            {
                size_t mid = left + (right - left) / 2;
                std::list<size_t>::iterator res_it = result.begin();
                std::advance(res_it, mid);
                std::list<int>::const_iterator d_it = data.begin();
                std::advance(d_it, *res_it);
                sort_count++;
                if (*d_it < val_to_insert)
                    left = mid + 1;
                else
                    right = mid;
            }

            std::list<size_t>::iterator insert_pos = result.begin();
            std::advance(insert_pos, left);
            result.insert(insert_pos, idx_to_insert);

            for (std::list<size_t>::iterator kit = chain_positions.begin(); kit != chain_positions.end(); ++kit)
            {
                if (*kit >= left)
                    (*kit)++;
            }
            inserted_count++;
        }
        prev_jacob = curr_jacob;
        jacob_idx++;
    }

    if (has_straggler)
    {
        std::list<int>::const_iterator val_it = data.begin();
        std::advance(val_it, straggler_idx);
        int val_to_insert = *val_it;

        size_t left = 0;
        size_t right = result.size();
        while (left < right)
        {
            size_t mid = left + (right - left) / 2;
            std::list<size_t>::iterator res_it = result.begin();
            std::advance(res_it, mid);
            std::list<int>::const_iterator d_it = data.begin();
            std::advance(d_it, *res_it);
            sort_count++;
            if (*d_it < val_to_insert)
                left = mid + 1;
            else
                right = mid;
        }
        std::list<size_t>::iterator insert_pos = result.begin();
        std::advance(insert_pos, left);
        result.insert(insert_pos, straggler_idx);
    }

    return result;
}

std::list<int> merge_insertion_sort_list(std::list<int> lst)
{
    if (lst.size() <= 1)
        return lst;

    std::list<size_t> indices;
    for (size_t i = 0; i < lst.size(); i++)
        indices.push_back(i);

    std::list<size_t> sorted_indices = sort_indices_list(lst, indices);

    std::list<int> result;
    for (std::list<size_t>::iterator it = sorted_indices.begin(); it != sorted_indices.end(); ++it)
    {
        std::list<int>::iterator data_it = lst.begin();
        std::advance(data_it, *it);
        result.push_back(*data_it);
    }

    return result;
}

void PmergeMe::MIsort_vec()
{
    sort_count = 0;
    clock_t start = clock();
    this->vec_ = merge_insertion_sort_vec(this->vec_);
    clock_t end = clock();
    double sort_time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000;
    std::cout << "vector sorted: ";
    for (size_t i = 0; i < this->vec_.size(); i++)
    {
        std::cout << this->vec_.at(i) << " ";
    }
    std::cout << std::endl;

    std::cout << "Time to process a range of " << this->vec_.size() << " elements with std::vector : " << sort_time << " us" << std::endl;

    std::cout << "[DEBUG] std::vector comparison count: " << sort_count << std::endl;
}

void PmergeMe::MIsort_list()
{
    sort_count = 0;
    clock_t start = clock();
    this->list_ = merge_insertion_sort_list(this->list_);
    clock_t end = clock();
    double sort_time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000;

    std::cout << "list sorted: ";
    for (std::list<int>::const_iterator it = this->list_.begin(); it != this->list_.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Time to process a range of " << this->list_.size() << " elements with std::list : " << sort_time << " us" << std::endl;

    std::cout << "[DEBUG] std::list comparison count: " << sort_count << std::endl;
}