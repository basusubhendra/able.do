#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

void print_contents( const boost::property_tree::ptree& pt)
{
    using boost::property_tree::ptree;
    for (const auto& x: pt )
    {
        std::cout << x.first << ": " << x.second.get_value<std::string>() << std::endl;
        print_contents(x.second);
    }
}

int main(int argc, char* argv[]) {
	char* json_file = strdup(argv[1]);
	printf("\nJSON FILE being read is %s\n", json_file);
	boost::property_tree::ptree pt;
	boost::property_tree::read_json(json_file, pt);
	print_contents(pt);
	return 0;
}
