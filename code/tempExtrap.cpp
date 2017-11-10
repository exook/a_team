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
        //if(testLeapYear(year)){
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

void makingMovingAverage(vector <Double_t> &y_movingAverage, vector <Double_t> &x_movingAverage, const vector<vector <float>> &averagesVector, vector <Double_t> &y_aroundMean,int groupSize){
    //int groupSize=30;//they used 5
    int counter1=0;
    int counter2=0;
    double sum=0;
    int initialYear=1723;//Hardcoded!
    for(int i=0;i<(int)averagesVector.size();i++){
        counter1+=1;
        sum+=y_aroundMean[i];
        if(counter1==groupSize){
            y_movingAverage.push_back(sum/groupSize);
            x_movingAverage.push_back(initialYear+(counter2*groupSize)+(groupSize/2));
            counter1=0;
            counter2+=1;
            sum=0;
            }
        }
        y_movingAverage.push_back(sum/counter1);
        x_movingAverage.push_back(initialYear+(counter2*groupSize)+(counter1/2));
        
}

float tempTrender::tempEx(int year){

    vector <vector <float>> dataVector;
    
    readAllData("../datasets/uppsala_tm_1722-2013.dat",dataVector);

    vector <vector<float>> averagesVector;

    averages(dataVector,averagesVector);

    float totalMean=totalAverage(averagesVector);
    cout<<"The total average is: "<<totalMean<<endl;

    vector <Double_t> x_aroundMean,y_aroundMean,y_above,y_below;
    separateData(totalMean,averagesVector,x_aroundMean,y_aroundMean,y_above,y_below);

    vector <Double_t> y_movingAverage1,x_movingAverage1;
    makingMovingAverage(y_movingAverage1, x_movingAverage1, averagesVector, y_aroundMean,30);
    
    TGraph *gr_average1 = new TGraph (x_movingAverage1.size(), &x_movingAverage1[0], &y_movingAverage1[0]);

    vector <Double_t> y_movingAverage2,x_movingAverage2;
    makingMovingAverage(y_movingAverage2, x_movingAverage2, averagesVector, y_aroundMean,11);
TGraph *gr_average2 = new TGraph (x_movingAverage2.size(), &x_movingAverage2[0], &y_movingAverage2[0]);

    TGraph *gr_above = new TGraph (x_aroundMean.size(), &x_aroundMean[0], &y_above[0]);
    TGraph *gr_below = new TGraph (x_aroundMean.size(), &x_aroundMean[0], &y_below[0]);

    TCanvas * c2= new TCanvas("c2", "random",1200,600);
    //c2->DrawFrame(1722,-3.0,2013,3.0);
    c2->DrawFrame(1722,-3,2013,3);
    TMultiGraph *mg = new TMultiGraph();

    //stringstream stream;
    //stream<<"Temperature (C), around "<<totalMean;
    //string s=stream.str();
    mg->GetXaxis()->SetTitle("Year");
    mg->GetYaxis()->SetTitle("Temperature (C),\n around given average");

    gr_above->SetFillColor(kRed+1);
    gr_below->SetFillColor(kBlue-4);

    mg->Add(gr_above,"B");
    mg->Add(gr_below,"B");

    gr_average1->SetLineWidth(4);

    mg->Add(gr_average1,"C");

    gr_average2->SetLineWidth(2);
    mg->Add(gr_average2,"C");
    mg->Draw();
    
    c2->SaveAs("extrapolatedDataNoFit.jpg");

    TF1* fitFunc = new TF1("fitFunc", "([0]*(x-1840)*cos([1]*x))", 1722, 2013);

    fitFunc->SetParameter(0, 0.6);
    fitFunc->SetParameter(1, 0.125);

    fitFunc->SetLineColor(kGreen-3);
    fitFunc->SetLineWidth(3);
    gr_average1->Fit(fitFunc);
    
    TLegend* leg = new TLegend(0.25,0.8,0.45,0.9);
    leg->SetFillStyle(0); //Hollow fill (transparent)
    leg->SetBorderSize(0); //Get rid of the border
    leg->SetNColumns(2);
    //leg->SetHeader("The Legend Title");
    leg->AddEntry(gr_average1,"Average 1","f");
    leg->AddEntry(gr_average2,"Average 2","f");
    leg->AddEntry(gr_above,"Above","f");
    leg->AddEntry(gr_below,"Below","f");
    leg->AddEntry(fitFunc, "fit", "l");
    leg->Draw();

    mg->Draw();

    float param0=fitFunc->GetParameter(0);
    float param1=fitFunc->GetParameter(1);
    //return param0*(year-1840)*cos(param1*year);
    //int year=2050;
    //cout<<param0*(year-1840)*cos(param1*year)<<endl;
    c2->SaveAs("extrapolatedDataFit.jpg");

    return param0*(year-1840)*cos(param1*year)+totalMean;

}
