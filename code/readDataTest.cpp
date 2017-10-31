#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main() {
	vector <vector <string> > data;
	string filePath;
	filePath = "/home/shi/Documents/a_team/datasets/smhi-opendata_Lund.csv";
	// open input file
	//ifstream file(filePath);
	ifstream file("smhi-opendata_Lund.csv");
	while (file) {
		string helpstring;
		while (getline(file, helpstring)) {
			if(!helpstring.empty()) {
		
			istringstream hhelpstring(helpstring);
			vector <string> record;
		
			while (hhelpstring) {
				string sss;
				if (!getline(hhelpstring, sss, ';')) break;
				record.push_back(sss);
			}
			data.push_back(record);
			}
		}
	}
	if (!file.eof()) {
		cerr<< "Fooey!\n";
	}
	cout<<"data size = "<<data.size()<<endl;
	for (int i=0; i<data.size(); i++){
		cout<<data[i][0]<<endl;
	}


	
}
