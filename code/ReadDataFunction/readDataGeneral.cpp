// Shi Qiu
// constructor for read_data_general

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "readDataGeneral.h"
using namespace std;
read_data_general::read_data_general(string filePath) {
	// open input file
	ifstream file(filePath.c_str());
	//ifstream file("smhi-opendata_Lund_test.csv")
	if (file) {
		cout << "Successfully reading data file" <<endl;
	}
	else {
		cout << "Unable to open data file. Check the directory. " << endl;
		
	}

	string helpstring;
	while (getline(file, helpstring)) {
		// skip empty lines
		if(!helpstring.empty()) {		
			istringstream hhelpstring(helpstring);
			vector <string> record_of_row;
			// break a row into different different parts based on the data structure
			while (hhelpstring) {
				string data_of_row;
				
				for (int ii = 0; ii<2; ii++) {
					// break time into year month and day
					if (!getline(hhelpstring, data_of_row, '-')) break;
					record_of_row.push_back(data_of_row);
				}
				
				if (!getline(hhelpstring, data_of_row, ';')) break;
				record_of_row.push_back(data_of_row);
				
				for (int iii = 0; iii<2; iii++) {
					if (!getline(hhelpstring, data_of_row, ':')) break;
					record_of_row.push_back(data_of_row);
				}
				
				while (true) {
					if (!getline(hhelpstring, data_of_row, ';')) break;
					record_of_row.push_back(data_of_row);
				}
			}
			data_.push_back(record_of_row);
		}
	}
	
	if (!file.eof()) {
		cerr<< "Error reading the file!\n";
	}
}
read_data_general::~read_data_general(){}
