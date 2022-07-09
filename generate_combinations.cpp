#include <string>
#include <set>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <gmp.h>
#include <algorithm>
using namespace std;
using boost::property_tree::ptree;

class JSON_Obj {
	protected:
		char* json_file;
		long max_attributes;
		long max_values_per_attribute;
		boost::property_tree::ptree pt;
	public:
		JSON_Obj(char* f, long max_attributes, long max_values_per_attribute);
		void upload_contents();
		boost::property_tree::ptree& getPropertyTree();
		long get_max_attributes();
		long get_max_values_per_attribute();
		void* parsePropertyTree(char**&, char***&, char***&);
};

JSON_Obj::JSON_Obj(char* f, long ma, long mvpa) {
	this->json_file = f;
	this->max_attributes = ma;
	this->max_values_per_attribute = mvpa;
}

long JSON_Obj::get_max_attributes() {
	return max_attributes;
}

long JSON_Obj::get_max_values_per_attribute() {
	return max_values_per_attribute;
}

char* _bin(long x) {
	if (x == 0) {
		char* ret = (char*) calloc(2, sizeof(char));
		ret[0] = '0';
		ret[1] = '\0';
		return ret;
	}
	std::string ss = "";
	while (x > 0) {
		int rem = x % 2;
		ss += boost::lexical_cast<std::string>(rem);
		x /= 2;
	}
	std::reverse(ss.begin(), ss.end());
	return strdup((char*) ss.c_str());
}

void* JSON_Obj::parsePropertyTree(char**& attributes, char***& values, char***& active_attributes) {
	attributes = (char**) calloc(max_attributes, sizeof(char*)); 
	values = (char***) calloc(max_attributes, sizeof(char**));
	for (int i = 0; i < max_attributes; ++i) {
		values[i] = (char**) calloc(max_values_per_attribute, sizeof(char*));
	}
	active_attributes = (char***) calloc(max_attributes, sizeof(char**));
	for (int i = 0; i < max_attributes; ++i) {
		active_attributes[i] = (char**) calloc(max_values_per_attribute, sizeof(char*));
	}
	boost::property_tree::ptree& pt = this->getPropertyTree();
	long attribute_count = -1;
	for(boost::property_tree::ptree::const_iterator v = pt.begin(); v != pt.end(); ++v) {
		std::string label = v->first;
		const auto& value_node = v->second;
		if (label == "name") {
		} else if (label == "attributes") {
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
						long value_count = 0;
						for (const auto& r: value_pt) {
							boost::property_tree::ptree val_node = r.second;
							for (const auto& h: val_node) {
								if (g == 0) {
									if (val1 != "" && val2 != "") {
										values[attribute_count][value_count] = (char*) calloc(val1.size()+1, sizeof(char));
										active_attributes[attribute_count][value_count] = (char*) calloc(val2.size()+1, sizeof(char));
										char* val1_cstr = strdup((char*) val1.c_str());
										strcpy(values[attribute_count][value_count], val1_cstr);
										char* val2_cstr = strdup((char*) val2.c_str());
										strcpy(active_attributes[attribute_count][value_count], val2_cstr);
									}
									val1 = h.second.get_value<std::string>();
								} else if (g == 1) {
									val2 = h.second.get_value<std::string>();
								}
								cout << g << "\t" << h.second.get_value<std::string>() << endl;
								g = 1 - g;
							}
							if (val1 != "" && val2 != "") {
								values[attribute_count][value_count] = (char*) calloc(val1.size()+1, sizeof(char));
								active_attributes[attribute_count][value_count] = (char*) calloc(val2.size()+1, sizeof(char));
								char* val1_cstr = strdup((char*) val1.c_str());
								strcpy(values[attribute_count][value_count], val1_cstr);
								char* val2_cstr = strdup((char*) val2.c_str());
								strcpy(active_attributes[attribute_count][value_count], val2_cstr);
							}
						}
					} else if (t == 0) {
						++attribute_count;
						name1 = x.first;
						boost::property_tree::ptree value_pt = x.second;
						label = value_pt.data();
						attributes[attribute_count] = (char*) calloc(label.size()+1, sizeof(char));
						attributes[attribute_count] = strdup((char*) label.c_str());
					}
					t = 1 - t;
					cout << endl;
				}
			}
		}
	}
	return nullptr;
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
	JSON_Obj* main_obj = new JSON_Obj(argv[1], atoi(argv[2]), atoi(argv[3]));
	main_obj->upload_contents();
	char** attributes = nullptr;
	char*** values = nullptr;
	char*** active_attributes = nullptr;
	void* ret =  main_obj->parsePropertyTree(attributes, values, active_attributes);
	long number_of_attributes = 0;
	for (int i = 0; i < main_obj->get_max_attributes(); ++i) {
		if (attributes[i] != nullptr) {
			++number_of_attributes;
		}
	}	       
	cout << number_of_attributes << endl;
	mpz_t pz;
	mpz_init(pz);
	mpz_ui_pow_ui(pz, 2, number_of_attributes);
	long total_combinations = mpz_get_ui(pz);
	cout << "\nTotal\t" << total_combinations << endl;
	for (int x = 1; x < total_combinations; ++x) {
		char* _bin_code = _bin(x);
		long l_bin_code = strlen(_bin_code);
		for (int y = 0; y < l_bin_code; ++y) {
			int bk = _bin_code[y] - '0';
			if (bk == '1' && strcmp(active_attributes[y][y],"active") == 0) {
				if (y == (l_bin_code - 1)) {
					cout << attributes[y] << "\t:\t" << values[y][y] << endl; 
				} else {
					cout << attributes[y] << "\t:\t" << values[y][y] << "\t|\t";
				}
			}	
		}
	}
	cout << endl;
	return 0;
}
