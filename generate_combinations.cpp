#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

class JSON_Obj {
	protected:
		char* json_file;
		boost::property_tree::ptree pt;
	public:
		JSON_Obj(char* f);
		void upload_contents();
		boost::property_tree::ptree& getPropertyTree();
		void print_contents(const boost::property_tree::ptree&);
};

JSON_Obj::JSON_Obj(char* f) {
	this->json_file = f;
}

void JSON_Obj::upload_contents() {
	boost::property_tree::read_json(json_file, pt);
}

boost::property_tree::ptree& JSON_Obj::getPropertyTree() {
	return this->pt;
}

void JSON_Obj::print_contents(const boost::property_tree::ptree& pt) {
    using boost::property_tree::ptree;
    for (const auto& x: pt )
    {
        std::cout << x.first << ": " << x.second.get_value<std::string>() << std::endl;
        print_contents(x.second);
    }
}

int main(int argc, char* argv[]) {
	JSON_Obj* main_obj = new JSON_Obj(argv[1]);
	main_obj->upload_contents();
	main_obj->print_contents(main_obj->getPropertyTree());
	return 0;
}
