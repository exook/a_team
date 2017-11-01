// Shi Qiu
// This is a demonstration of how to use this class in the main function

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
	vector <vector<string> > getData(vector <vector<string> > data) {data = data_; return data;}
};

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

int main() {
	
	string filePath;
	filePath = "smhi-opendata_Lund_test.csv";
	read_data_general read_data_generala(filePath);
	
	vector <vector <string> > data;
	data = read_data_generala.getData(data);
	// output result
	cout<<"data size (row) = "<<data.size()<<endl;
	cout<<"data size (first column) = "<<data[1].size()<<endl;
	
	for (int i=0; i<data.size(); i++){
		for (int j=0; j<data[i].size(); j++) {
		cout<<data[i][j]<<"  ";
		}
		cout<<endl;
	}
}
