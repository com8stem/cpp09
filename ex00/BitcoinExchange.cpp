#include "BitcoinExchange.hpp"

#define DATA_FILEPATH "./data.csv"

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const std::string input_filename)
{
    this->filename_ = input_filename;
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : filename_(other.filename_), database_(other.database_)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
    {
        this->filename_ = other.filename_;
        this->database_ = other.database_;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

std::ifstream open_csv()
{
    std::ifstream ifs(DATA_FILEPATH);

    if (!ifs)
    {
        std::cerr << "Error: could not create database." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return ifs;
}

void BitcoinExchange::read_csv(std::ifstream &ifs)
{
    std::string::size_type pos;
    std::string line;
    std::string key;
    std::string value;

    std::getline(ifs, line);
    for (; std::getline(ifs, line);)
    {
        pos = line.find(',');
        key = line.substr(0, pos);
        value = line.substr(++pos);
        database_[key] = atof(value.c_str());
    }
}

void BitcoinExchange::create_database()
{
    std::ifstream ifs;
    ifs = open_csv();
    read_csv(ifs);
}

std::ifstream BitcoinExchange::open_txt()
{
    std::ifstream ifs(filename_.c_str());
    if (!ifs)
    {
        std::cerr << "Error: could not open file." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return ifs;
}

size_t count_char(const char *str, char c)
{
    size_t count = 0;

    while (*str != '\0')
    {
        if (*str == c)
            count++;
        str++;
    }
    return count;
}

bool is_num(const char *str)
{
    while (*str != '\0')
    {
        if (!isdigit(*str))
            return false;
        str++;
    }
    return true;
}

bool is_valid_key(std::string key)
{
    size_t count_hyphen = count_char(key.c_str(), '-');

    if (count_hyphen != 2)
    {
        std::cerr << "Error: bad input => " << key << std::endl;
        return false;
    }
    if (count_char(key.c_str(), '.') != 0)
    {
        std::cerr << "Error: bad input => " << key << std::endl;
        return false;
    }

    std::string::size_type pos, pos2;
    pos = key.find("-");
    pos2 = key.rfind("-");

    std::string year = key.substr(0, pos);
    std::string month = key.substr(pos + 1, pos2 - pos - 1);
    std::string day = key.substr(pos2 + 1);
    if (!(is_num(year.c_str()) && is_num(month.c_str()) && is_num(day.c_str())))
    {
        std::cerr << "Error: bad input => " << key << std::endl;
        return false;
    }

    long y = atol(year.c_str());
    long m = atol(month.c_str());
    long d = atol(day.c_str());
    if ((y < 2009 || y > 2022) || (m < 1 || m > 12) || (d < 1 || d > 31))
    {
        std::cerr << "Error: bad input => " << key << std::endl;
        return false;
    }
    return true;
}

bool is_valid_value(std::string value)
{
    float l_value = atof(value.c_str());

    if (l_value < 0)
    {
        std::cerr << "Error: not a positive number." << std::endl;
        return false;
    }
    if (l_value > 1000)
    {
        std::cerr << "Error: too large a number." << std::endl;
        return false;
    }
    return true;
}

void BitcoinExchange::search_database()
{
    std::ifstream ifs;

    ifs = open_txt();

    std::string line;
    std::string::size_type pos;
    std::string key;
    std::string value;

    std::getline(ifs, line);
    for (; std::getline(ifs, line);)
    {
        pos = line.find(" | ");
        if (pos == std::string::npos)
        {
            std::cerr << "Error: bad input => (could not display)" << std::endl;
            continue;
        }
        key = line.substr(0, pos);
        value = line.substr(pos + 3);

        if (!(is_valid_key(key) && is_valid_value(value)))
            continue;

        std::map<std::string, float>::iterator it = database_.lower_bound(key);
        if (it == database_.begin())
        {
            std::cerr << "Error: no earlier date found => " << key << std::endl;
            continue;
        }
        if (it == database_.end() || it->first != key)
        {
            --it;
        }
        std::cout << key << " => " << value << " = ";
        std::cout << it->second * atof(value.c_str()) << std::endl;
    }
}
