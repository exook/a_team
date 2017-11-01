// Shi Qiu
// general read data function head file
// data is stored in a two dimension array
#ifndef READDATAGENERAL_H
#define READDATAGENERAL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class read_data_general {
private:
	vector <vector <string> > data_;
	string filePath;
public:
	read_data_general(string filePath);
	~read_data_general();
	vector <vector<string> > getData(vector <vector<string> > data) {
		data = data_; 
		return data;
	}
};
#endif
