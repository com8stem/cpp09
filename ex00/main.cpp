#include "BitcoinExchange.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }

    BitcoinExchange exchange(argv[1]);
	try 
	{
		exchange.create_database();
		exchange.search_database();
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
    return 0;
}
