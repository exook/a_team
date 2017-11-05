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

void readDataOld(vector<vector<float>>* dataVectorPointer){	
	ifstream file("/home/alexander/Desktop/a_team/datasets/uppsala_tm_1722-2013.dat");
	//check if opened correctly
    if (!file) {
        cout << "Error could not read data file" << endl;
    }
    else {
        cout << "Succesfully opened data file" << endl;
    }

    string value;

    string line;
    while (getline(file,line)){
        istringstream helpstring(line);
        vector <float> row;
        while(getline(helpstring,value,' ')){
            if (value.find_first_not_of(' ') != std::string::npos){
                float number=strtof(value.c_str(),NULL);
                row.push_back(number);
            }
        
        }
        dataVectorPointer->push_back(row);
    }
    file.close();
}

int isLeapYear(int year){
    if((year % 4 == 0 && year % 100 != 0) || ( year % 400 == 0)){
        return 1;
    }
    else{
        cout<<"regular year"<<endl;
        return 0;
    }
}
/*
void totalAverage(vector<vector<string>>* dataVectorPointer){
    for(size_t i = 0; i < dataVectorPointer->size(); ++i){
        //cout<<dataVectorPointer->at(i).at(4)<<endl;
        yearlySum=0;
            for(int j;j<365;j++){
                yearlySum=yearlySum+dataVectorPointer->at(i).at(4);
            }
        yearlySum/365;
    }
    
}
*/
void tempTrender::tempEx(){

    vector <vector <float>> dataVector;
    vector <vector <float>> *dataVectorPointer=&dataVector;
    
    readDataOld(dataVectorPointer);

	//vector <vector <string> > data;
	//data=readDataOld();
    TH1D* hist = new TH1D("data", ";x;N", 100, -50, 50);

    for(size_t i = 0; i < dataVector.size(); ++i){
        hist->Fill(dataVector.at(i).at(4));
        //hist->Fill(strtof(dataVector.at(i).at(4).c_str(),NULL));
        
    }

    TCanvas * c1= new TCanvas("c1", "random",5,5,800,600);
    hist->Draw();

    //totalAverage(dataVectorPointer);
    //cout<<totalAverage<<endl;

}
