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
#include <TLegend.h>
#include <TMultiGraph.h>
using namespace std;

void readDataOld(vector<vector<float>> &dataVector){	
	ifstream file("../datasets/uppsala_tm_1722-2013.dat");
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
        dataVector.push_back(row);
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

void averages(vector<vector<float>> &dataVector,vector <vector<float>> &averagesVector){

    ofstream writer;
    writer.open("bug.txt");
    
    int row=0;
    for(size_t i = 0; i < dataVector.size(); ++i){
        row=i-1;
        if(dataVector.at(i).at(1)==1 && dataVector.at(i).at(2)==1){
            break;
        }
        else{
        }
    }

    int initialYear=dataVector.at(row).at(0);
    int endYear=dataVector.at(dataVector.size()-1).at(0);

    vector <float> thisYear;

    for(int year = initialYear+1; year <= endYear;year++){
        float yearlySum=0;
        vector <float> thisYear;
        if(isLeapYear(year)){
            for(int day=1;day<=366;day++){
                yearlySum+=dataVector.at(row).at(4);
                row++;
            }
        thisYear.push_back(year);
        thisYear.push_back(yearlySum/366);
        averagesVector.push_back(thisYear);
        }
        else{
            for(int day=1;day<=365;day++){
                yearlySum+=dataVector.at(row).at(4);
                row++;
            }
        thisYear.push_back(year);
        thisYear.push_back(yearlySum/366);
        averagesVector.push_back(thisYear);
        }
    }

    writer.close();
}

float totalAverage(vector <vector<float>> &averagesVector){
    float totalSum=0;
    for(size_t i = 0; i < averagesVector.size(); ++i){
        totalSum+=averagesVector.at(i).at(1);
    }
    return totalSum/averagesVector.size();
} 

void separateData(const float totalMean,const vector<vector <float>> &averagesVector,vector <Double_t> &x_aroundMean,vector <Double_t> &y_aroundMean,vector <Double_t> &y_above,vector <Double_t> &y_below){
   Int_t n = averagesVector.size();
   for (Int_t i=0; i<n; i++) {
        x_aroundMean.push_back(averagesVector.at(i).at(0));
        y_aroundMean.push_back(averagesVector.at(i).at(1)-totalMean);
        if(y_aroundMean[i]<0){
            y_below.push_back(y_aroundMean[i]);
            y_above.push_back(0);
        }
        if(y_aroundMean[i]>=0){
            y_above.push_back(y_aroundMean[i]);
            y_below.push_back(0);
        }
   }

}

void tempTrender::tempEx(){

    vector <vector <float>> dataVector;
    
    readDataOld(dataVector);

    vector <vector<float>> averagesVector;

    averages(dataVector,averagesVector);

    float totalMean=totalAverage(averagesVector);

    vector <Double_t> x_aroundMean,y_aroundMean,y_above,y_below;
    separateData(totalMean,averagesVector,x_aroundMean,y_aroundMean,y_above,y_below);

    int groupSize=30;//they used 5
    vector <Double_t> y_movingAverage,x_movingAverage;

    int counter1=0;
    int counter2=0;
    double sum=0;
    int initialYear=1723;//Hardcoded!
    for(int i=0;i<averagesVector.size();i++){
        counter1+=1;
        sum+=y_aroundMean[i];
        if(counter1==groupSize){
            y_movingAverage.push_back(sum/groupSize);
            x_movingAverage.push_back(initialYear+(counter2*groupSize)+(groupSize/2));
            counter1=0;
            counter2+=1;
            cout<<"Average: "<<y_movingAverage[i/groupSize]<<endl;
            sum=0;
        }
    }
    y_movingAverage.push_back(sum/counter1);
    x_movingAverage.push_back(initialYear+(counter2*groupSize)+(counter1/2));

    TGraph *gr_average = new TGraph (x_movingAverage.size(), &x_movingAverage[0], &y_movingAverage[0]);
    TGraph *gr_above = new TGraph (x_aroundMean.size(), &x_aroundMean[0], &y_above[0]);
    TGraph *gr_below = new TGraph (x_aroundMean.size(), &x_aroundMean[0], &y_below[0]);

    TCanvas * c2= new TCanvas("c2", "random",1200,600);
    c2->DrawFrame(1722,-3.0,2013,3.0);
    TMultiGraph *mg = new TMultiGraph();

    gr_above->SetFillColor(kRed-3);
    
    gr_below->SetFillColor(kBlue-3);

    gr_average->SetLineWidth(3);
    gr_average->SetMarkerStyle(8);
    gr_average->SetMarkerSize(1.5);

    mg->Add(gr_above,"B");
    mg->Add(gr_below,"B");
    mg->Add(gr_average,"PC");

    mg->GetXaxis()->SetTitle("Year");
    mg->GetYaxis()->SetTitle("Temperature (C)");  

    mg->Draw();

    TF1* fitFunc = new TF1("fitFunc", "([0]*(x-1840)*cos([1]*x))", 1722, 2013);

    fitFunc->SetParameter(0, 0.6);
    fitFunc->SetParameter(1, 0.125);

    fitFunc->SetLineColor(kGreen-3);
    fitFunc->SetLineWidth(3);
    gr_average->Fit(fitFunc);
    
    TLegend* leg = new TLegend(0.25,0.8,0.45,0.9);
    leg->SetFillStyle(0); //Hollow fill (transparent)
    leg->SetBorderSize(0); //Get rid of the border
    leg->SetNColumns(2);
    //leg->SetHeader("The Legend Title");
    leg->AddEntry(gr_average,"Average","f");
    leg->AddEntry(gr_above,"Above","f");
    leg->AddEntry(gr_below,"Below","f");
    leg->AddEntry(fitFunc, "fit", "l");
    leg->Draw();

    double_t param0=fitFunc->GetParameter(0);
    double_t param1=fitFunc->GetParameter(1);
    //return param0*(year-1840)*cos(param1*year);
    int year=2050;
    cout<<param0*(year-1840)*cos(param1*year)<<endl;
    c2->SaveAs("extrapolatedData.jpg");

}
