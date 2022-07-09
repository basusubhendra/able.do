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
#include <algorithm>
#include <gmp.h>
#include <unordered_map>
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
		void* parsePropertyTree(char**&, long*&, long*&, char***&, char***&);
		void* generate_combinations(long* value_counts2, long number_of_attributes, vector<char*>& combinations, char*** active_attributes);
	        void* print_combinations(vector<char*> combinations, long num_of_combinations, long number_of_attributes, char** attributes);
};

JSON_Obj::JSON_Obj(char* f, long ma, long mvpa) {
	this->json_file = f;
	this->max_attributes = ma;
	this->max_values_per_attribute = mvpa;
}

long JSON_Obj::get_max_attributes() {
	return max_attributes;
}

void* JSON_Obj::print_combinations(vector<char*> combinations, long num_of_combinations, long number_of_attributes, char** attributes) {
	cout << "\nTotal\t" << num_of_combinations << endl;
	for (int x = 0; x < num_of_combinations; ++x) {
		for (long y = 0; y < number_of_attributes; ++y) {
			cout << attributes[y] << "\t:\t" ;
			cout << combinations[x][y];
			if (y != (number_of_attributes - 1)) {
				cout << "\t|\t";
			} else {
                                cout << "\n";
			}
		}
	}
	return nullptr;
}

long JSON_Obj::get_max_values_per_attribute() {
	return max_values_per_attribute;
}

void* JSON_Obj::generate_combinations(long* value_counts2, long number_of_attributes, vector<char*>& combinations, char*** active_attributes) {
	long max_val = -1;
	for (long i = 0; i < number_of_attributes; ++i) {
		if (value_counts2[i] > max_val) {
			max_val = value_counts2[i];
		}
	}
	std::string max_val_string = "";
	for (long i = 0; i < number_of_attributes; ++i) {
		max_val_string += boost::lexical_cast<std::string>(max_val);
	}
	std::string min_val_string = "";
	for (long i = 0; i < number_of_attributes; ++i) {
		min_val_string += boost::lexical_cast<std::string>(1);
	}
	long range_min = atoi((char*) min_val_string.c_str());
	long range_max = atoi((char*) max_val_string.c_str());
#ifdef _DEBUG
	cout << range_min << "\t" << range_max << endl;
#endif
	vector<char*> range_of_integers;
	for (long i = range_min; i <= range_max; ++i) {
		char* test_string = strdup((char*) boost::lexical_cast<std::string>(i).c_str());
		if (strchr(test_string, '0') != nullptr) {
			continue;
		}
		range_of_integers.push_back(test_string);
	}
	for (long i = 0; i < range_of_integers.size(); ++i) {
		int hit = 0;
		for (long x = 0; x < number_of_attributes; ++x) {
			for (long y = 0; y < value_counts2[x]; ++y) {
				if (range_of_integers[i][x]-'0' == (y + 1)) {
					++hit;
				}
			}
		}
		if (hit == number_of_attributes) {
			combinations.push_back(range_of_integers[i]);
		}
	}
	return nullptr;
}

void* JSON_Obj::parsePropertyTree(char**& attributes, long*& value_counts1, long*& value_counts2, char***& values, char***& active_attributes) {
	attributes = (char**) calloc(max_attributes, sizeof(char*)); 
	value_counts1 = (long*) calloc(max_attributes, sizeof(long));
	value_counts2 = (long*) calloc(max_attributes, sizeof(long));
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
						long value_count1 = 0, value_count2 = 0;
						for (const auto& r: value_pt) {
							boost::property_tree::ptree val_node = r.second;
							for (const auto& h: val_node) {
								if (g == 0) {
									val1 = h.second.get_value<std::string>();
										values[attribute_count][value_count2] = (char*) calloc(val1.size()+1, sizeof(char));
										char* val1_cstr = strdup((char*) val1.c_str());
										strcpy(values[attribute_count][value_count2], val1_cstr);
								                value_counts2[attribute_count] = value_count2+1;
										++value_count2;
								} else if (g == 1) {
									val2 = h.second.get_value<std::string>();
										active_attributes[attribute_count][value_count1] = (char*) calloc(val2.size()+1, sizeof(char));
										char* val2_cstr = strdup((char*) val2.c_str());
										strcpy(active_attributes[attribute_count][value_count1], val2_cstr);
								                value_counts1[attribute_count] = value_count1+1;
										++value_count1;
								}
#ifdef _DEBUG
										cout << value_count1 << "\t" << value_count2 << endl;
								cout << g << "\t" << h.second.get_value<std::string>() << endl;
#endif
								g = 1 - g;
							}
						}
#ifdef _DEBUG
						cout << value_count1 << "\t" << value_count2 << endl;
#endif
					} else if (t == 0) {
						++attribute_count;
						name1 = x.first;
						boost::property_tree::ptree value_pt = x.second;
						label = value_pt.data();
						attributes[attribute_count] = (char*) calloc(label.size()+1, sizeof(char));
						attributes[attribute_count] = strdup((char*) label.c_str());
					}
					t = 1 - t;
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

#ifdef _DEBUG
void print_contents(const boost::property_tree::ptree& pt) {
	using boost::property_tree::ptree;
	for (const auto& x: pt ) {
		std::cout << x.first << ": " << x.second.get_value<std::string>() << std::endl;
		print_contents(x.second);
	}
}
#endif

int main(int argc, char* argv[]) {
	if (argc < 4) {
		printf("\nUsage: ./generate_combinations ./input.json 10 20\n");
		exit(3);
	}
	long max_attributes = atoi(argv[2]);
	long max_attributes_per_value = atoi(argv[3]);
	JSON_Obj* main_obj = new JSON_Obj(argv[1], max_attributes, max_attributes_per_value);
	main_obj->upload_contents();
	char** attributes = nullptr;
	char*** values = nullptr;
	long* value_counts1 = nullptr;
	long* value_counts2 = nullptr;
	char*** active_attributes = nullptr;
	void* ret =  main_obj->parsePropertyTree(attributes, value_counts1, value_counts2, values, active_attributes);
	long number_of_attributes = 0;
	for (int i = 0; i < main_obj->get_max_attributes(); ++i) {
		if (attributes[i] != nullptr) {
			++number_of_attributes;
		}
	}	       
#ifdef _DEBUG
	cout << number_of_attributes << endl;
#endif
	vector<char*> combinations;
	main_obj->generate_combinations(value_counts2, number_of_attributes, combinations, active_attributes);
#ifdef _DEBUG
	for (int i = 0; i < combinations.size(); ++i) {
		cout << combinations[i] << endl;
	}
#endif
	long num_of_combinations = combinations.size();
	main_obj->print_combinations(combinations, num_of_combinations, number_of_attributes, attributes);
	return 0;
}
