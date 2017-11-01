// Shi Qiu
// This is just a test
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
// Can be used to read all weather data except for Uppsala which has a diff format
/*class readDataGeneral {
	public readDataGeneral(string filePath)
	
}*/


vector <vector<string> > readData(string filePath) {
	vector <vector <string> > data;
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
			data.push_back(record_of_row);
		}
	}
	
	if (!file.eof()) {
		cerr<< "Error reading the file!\n";
	}
	
	// print out data
	/*cout<<"data size = "<<data.size()<<endl;
	cout<<"data size = "<<data[1].size()<<endl;
	for (int i=0; i<data.size(); i++){
		for (int j=0; j<data[i].size(); j++) {
		cout<<data[i][j]<<"  ";
		}
		cout<<endl;
	}*/
	return data;
	
}
int main() {
	
	string filePath;
	filePath = "/home/shi/Documents/a_team/code/smhi-opendata_Lund_test.csv";
	vector <vector <string> > data = readData(filePath);
	cout<<"data size = "<<data.size()<<endl;
	cout<<"data size = "<<data[1].size()<<endl;
	for (int i=0; i<data.size(); i++){
		for (int j=0; j<data[i].size(); j++) {
		cout<<data[i][j]<<"  ";
		}
		cout<<endl;
	}
}
