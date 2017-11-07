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
#include <TGraph.h>

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
        return 0;
    }
}

void averages(vector<vector<float>>* dataVectorPointer,vector <vector<float>>* averagesVectorPointer){


    //cout<<initialYear<<","<<endYear<<endl;

    ofstream writer;
    writer.open("bug.txt");
    
    int row=0;
    for(size_t i = 0; i < dataVectorPointer->size(); ++i){
        row=i-1;
        if(dataVectorPointer->at(i).at(1)==1 && dataVectorPointer->at(i).at(2)==1){
            break;
        }
        else{
            //cout<<"No first of january found"<<endl;
        }
    }

    int initialYear=dataVectorPointer->at(row).at(0);
    int endYear=dataVectorPointer->at(dataVectorPointer->size()-1).at(0);

    vector <float> thisYear;

    for(int year = initialYear; year < endYear;year++){//warning: comparison between signed and unsigned integer expressions [-Wsign-compare
        float yearlySum=0;
        vector <float> thisYear;
        if(isLeapYear(year)){
            for(int day=1;day<=366;day++){
                yearlySum+=dataVectorPointer->at(row).at(4);
                row++;
                //writer<<year<<":"<<day<<endl;
                //writer<<dataVectorPointer->at(row).at(0)<<":"<<dataVectorPointer->at(row).at(1)<<":"<<dataVectorPointer->at(row).at(2)<<endl<<endl;
            }
        thisYear.push_back(year);
        thisYear.push_back(yearlySum/366);
        averagesVectorPointer->push_back(thisYear);
        }
        else{
            for(int day=1;day<=365;day++){
                yearlySum+=dataVectorPointer->at(row).at(4);
                row++;
                //writer<<year<<":"<<day<<endl;
                //writer<<dataVectorPointer->at(row).at(0)<<":"<<dataVectorPointer->at(row).at(1)<<":"<<dataVectorPointer->at(row).at(2)<<endl<<endl;
            }
        thisYear.push_back(year);
        thisYear.push_back(yearlySum/366);
        averagesVectorPointer->push_back(thisYear);
        }
    }

    writer.close();
}

float totalAverage(vector <vector<float>>* averagesVectorPointer){
    float totalSum=0;
    for(size_t i = 0; i < averagesVectorPointer->size(); ++i){
        totalSum+=averagesVectorPointer->at(i).at(1);
    }
    return totalSum/averagesVectorPointer->size();
    

}

void tempTrender::tempEx(){

    vector <vector <float>> dataVector;
    vector <vector <float>> *dataVectorPointer=&dataVector;
    
    readDataOld(dataVectorPointer);

    vector <vector<float>> averagesVector;
    vector <vector<float>> *averagesVectorPointer=&averagesVector; 

    averages(dataVectorPointer,averagesVectorPointer);

    //TH1D* hist = new TH1D("data", ";x;N", 100, 0, 10);
    //for(size_t i = 0; i < averagesVector.size(); ++i){
    //    hist->Fill(averagesVector.at(i).at(1));
    //}
    //TCanvas * c1= new TCanvas("c1", "random",5,5,800,600);
    //hist->Draw();

    float totalMean=totalAverage(averagesVectorPointer);
    //cout<<totalMean<<endl;

   Int_t n = averagesVector.size();
   Double_t x[n], y[n],y_above[n],y_below[n];
   for (Int_t i=0; i<n; i++) {
        x[i] = averagesVector.at(i).at(0);
        y[i] = averagesVector.at(i).at(1)-totalMean;
        if(y[i]<0){
            y_below[i]=y[i];
            y_above[i]=0;
        }
        if(y[i]>=0){
            y_above[i]=y[i];
            y_below[i]=0;
        }
   }
    TGraph *gr1 = new TGraph (n, x, y);
    TGraph *gr_above = new TGraph (n, x, y_above);
    TGraph *gr_below = new TGraph (n, x, y_below);

/*
    TCanvas * c_working= new TCanvas("c1", "random",5,5,1200,600);
    gr1->SetFillColor(40);
    gr1->GetXaxis()->SetTitle("X-Axis");
    gr1->GetXaxis()->SetLimits(1722,2013);
    gr1->GetYaxis()->SetTitle("Y-Axis");
    //gr1->GetXaxis()->SetLimits(3.0,-3.0);
    gr1->Draw("AB");
*/

    TCanvas * c2= new TCanvas("c2", "random",5,5,1200,600);
    c2->DrawFrame(1722,-3.0,2013,3.0);

    gr_above->SetTitle("graph title;x title;y title");
        
    gr_above->SetFillColor(2);
    gr_above->Draw("B");
    
    gr_below->SetFillColor(4);
    gr_below->Draw("B");
    
}

/*
    int initialYear=dataVectorPointer->at(0).at(0);
    int endYear=dataVectorPointer->at(dataVectorPointer->size()-1).at(0);

    for(int row = 0; row < (endYear-initialYear);row++){
        for(float i=0;i<averagesVector.at(row);i+=0.01)
            cout<<row+initialYear<<endl;
            hist->Fill(row+initialYear);
    }
*/
