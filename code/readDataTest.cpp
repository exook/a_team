#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void readData(string filePath) {
	vector <vector <string> > data;
	// open input file
	//ifstream file(filePath);
	ifstream file("smhi-opendata_Lund_test.csv");
	if (file) {
		cout << "Successfully reading data file" <<endl;
	}
	else {
		cout << "Unable to open data file. Check the directory. " << endl;
		return;
	}

	string helpstring;
	while (getline(file, helpstring)) {
		// skip empty lines
		if(!helpstring.empty()) {		
			istringstream hhelpstring(helpstring);
			vector <string> record_of_row;
		
			while (hhelpstring) {
				string data_of_row;
				// break a row into different i
				if (!getline(hhelpstring, data_of_row_separated_by_semicolom, ';')) break;
				
				record_of_row.push_back(data_of_row);
			}
			data.push_back(record_of_row);
		}
	}
	
	if (!file.eof()) {
		cerr<< "Error reading the file!\n";
	}
	
	cout<<"data size = "<<data.size()<<endl;
	cout<<"data size = "<<data[1].size()<<endl;
	for (int i=0; i<data.size(); i++){
		cout<<data[i][0]<<endl;
	}
}
int main() {
	string filePath;
	filePath = "/home/shi/Documents/a_team/datasets/smhi-opendata_Lund.csv";
	readData(filePath);
}
