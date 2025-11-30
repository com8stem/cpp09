#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <map>
#include <exception>
#include <cctype>

class BitcoinExchange
{
    private:
        std::string filename_;
        std::map<std::string, float> database_;
        BitcoinExchange();

    public:
        BitcoinExchange(const std::string input_filename);
        BitcoinExchange(const BitcoinExchange &);
        BitcoinExchange &operator=(const BitcoinExchange &);
        ~BitcoinExchange();
        void read_csv(std::ifstream &ifs);
        std::ifstream open_txt();
        void create_database();
        void search_database();
};

#endif