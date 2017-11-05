#include <iostream>
#include "tempTrender.h"
#include <sstream>
// ROOT library obejcts
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <fstream>
#include <TRandom3.h>

using namespace std;

vector<vector<string>> readDataOld(){	
	ifstream file("/home/alexander/Desktop/a_team/datasets/uppsala_tm_1722-2013.dat");
	//check if opened correctly
    if (!file) {
        cout << "Error could not read data file" << endl;
    }
    else {
        cout << "Succesfully opened data file" << endl;
    }

    string value;
    vector <vector <string>> dataVector;
    string line;

    while (getline(file,line)){
        istringstream helpstring(line);
        vector <string> row;
        while(getline(helpstring,value,' ')){
            if (value.find_first_not_of(' ') != std::string::npos){
                row.push_back(value);
            }
        
        }
        dataVector.push_back(row);
    }
    file.close();
    return dataVector;
}


void tempTrender::tempEx(){
	vector <vector <string> > data;
	data=readDataOld();
    //gRandom=new TRandom3();
    TH1D* hist = new TH1D("data", ";x;N", 100, -50, 50);

    for(size_t i = 0; i < data.size(); ++i){
        //cout<<data.at(i).at(4);
        //hist->Fill(gRandom->Gaus(65.0, 5.0));
        float value;
        //value=string_to_float(data.at(i).at(4));
        hist->Fill(strtof(data.at(i).at(4).c_str(),NULL));
    }

    TCanvas * c1= new TCanvas("c1", "random",5,5,800,600);
    hist->Draw();

}
