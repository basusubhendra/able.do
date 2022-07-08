#include <string>
#include <set>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
using namespace std;
using boost::property_tree::ptree;

class JSON_Obj {
	protected:
		char* json_file;
		boost::property_tree::ptree pt;
	public:
		JSON_Obj(char* f);
		void upload_contents();
		boost::property_tree::ptree& getPropertyTree();
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

void print_contents(const boost::property_tree::ptree& pt) {
	using boost::property_tree::ptree;
	for (const auto& x: pt ) {
		std::cout << x.first << ": " << x.second.get_value<std::string>() << std::endl;
		print_contents(x.second);
	}
}

int main(int argc, char* argv[]) {
	JSON_Obj* main_obj = new JSON_Obj(argv[1]);
	main_obj->upload_contents();
	boost::property_tree::ptree pt = main_obj->getPropertyTree();
	for(boost::property_tree::ptree::const_iterator v = pt.begin(); v != pt.end(); ++v) {
		std::string label = v->first;
		const auto value_node = v->second;
		if (label == "attributes") {
			for (const auto x: value_node) {
				const auto value_pair = x.second;
				std::string name1 = "";
				std::string name2 = "";
				int t = 0;
				for (const auto x: value_pair) {
					if (t == 1) {
						name1 = x.first;
						const auto value_pt = x.second;
						cout << name1 << endl;
					} else if (t == 0) {
						name1 = x.first;
						const auto value_pt_2 = x.second;
						cout << name1 << endl;
					}
					t = 1 - t;
				}
			}
		}
	}
	return 0;
}
