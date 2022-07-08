#include <string>
#include <set>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
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
		std::unordered_map<std::string, vector<char** >* >* parsePropertyTree();
};

JSON_Obj::JSON_Obj(char* f) {
	this->json_file = f;
}

std::unordered_map<std::string, vector<char** >* >* JSON_Obj::parsePropertyTree() {
	std::unordered_map<std::string, vector<char** >* >* attribute_map = (std::unordered_map<std::string, vector<char** >* >*) calloc(1, sizeof(std::unordered_map<std::string, vector<char** >* >));
	boost::property_tree::ptree& pt = this->getPropertyTree();
	for(boost::property_tree::ptree::const_iterator v = pt.begin(); v != pt.end(); ++v) {
		std::string label = v->first;
		const auto& value_node = v->second;
		if (label == "attributes") {
			for (const auto& x: value_node) {
				const auto& value_pair = x.second;
				std::string name1 = "";
				std::string name2 = "";
				int t = 0;
				std::string label = "";
				std::string val1 = "";
				std::string val2 = "";
				for (const auto& x: value_pair) {
					if (t == 1) {
						name1 = x.first;
						const auto& value_pt = x.second;
						int g = 0;
						for (const auto& r: value_pt) {
							boost::property_tree::ptree val_node = r.second;
							for (const auto& h: val_node) {
								if (g == 0) {
									if (val1 != "" && val2 != "") {
										std::unordered_map<std::string, vector<char** >* >::iterator it = attribute_map->find(label);
										std::vector<char** >* value_array = (*it).second;
										char** val_2 = (char**) calloc(2, sizeof(char*));
										val_2[0] = (char*) calloc(val1.size()+1, sizeof(char));
										val_2[0]= strdup((char*)val1.c_str());
										val_2[1] = (char*) calloc(val2.size()+1, sizeof(char));
										val_2[1]= strdup((char*)val2.c_str());
									}
									val1 = h.second.get_value<std::string>();
								} else if (g == 1) {
									val2 = h.second.get_value<std::string>();
								}
								cout << g << "\t" << h.second.get_value<std::string>() << endl;
								g = 1 - g;
							}
							if (val1 != "" && val2 != "") {
								std::unordered_map<std::string, vector<char** >* >::iterator it = attribute_map->find(label);
								std::vector<char** >* value_array = (*it).second;
								char** val_2 = (char**) calloc(2, sizeof(char*));
								val_2[0] = (char*) calloc(val1.size()+1, sizeof(char));
								val_2[0]= strdup((char*)val1.c_str());
								val_2[1] = (char*) calloc(val2.size()+1, sizeof(char));
								val_2[1]= strdup((char*)val2.c_str());
							}
						}
					} else if (t == 0) {
						name1 = x.first;
						boost::property_tree::ptree value_pt = x.second;
						label = value_pt.data();
						vector<char** >* values = (vector<char** >* ) calloc(1, sizeof(vector<char** >));
						attribute_map->insert(std::make_pair(value_pt.data(), values));
					}
					t = 1 - t;
					cout << endl;
				}
			}
		}
	}
	return attribute_map;
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
	main_obj->parsePropertyTree();
	std::unordered_map<std::string, vector<char** >* >* attribute_map =  main_obj->parsePropertyTree();
	long number_of_attributes = attribute_map->size();
	cout << number_of_attributes << endl;
	return 0;
}
