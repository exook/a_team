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

/**
This function takes raw data of a 2D vector [year][temperature] and calculates the average temperature per year givng a smaller 2D vector [year][average temperature]
@param: 2D vector input, 2D vector output
*/
void averages(vector<vector<float>> &dataVector,vector <vector<float>> &averagesVector){
    int row=0;//Keep track of the row in the raw data
    //This for loop finds the row where the first full year start. i.e a year that starts with 1st of January
    for(size_t i = 0; i < dataVector.size(); ++i){
        row=i-1;//The row is i-1 inorder to give the row of 1st of january and not the 2nd
        if(dataVector.at(i).at(1)==1 && dataVector.at(i).at(2)==1){
            break;
        }
        else{
        }
    }

    int initialYear=dataVector.at(row).at(0);//Find the first full year using the row determined before
    int endYear=dataVector.at(dataVector.size()-1).at(0);//Find the last year. Does not have to be full

    //This nested for loop divides all temperature values into corresponding years by checking if a year is leap-year or not
    for(int year = initialYear+1; year <= endYear;year++){
        float yearlySum=0;
        vector <float> thisYear;//Dummy vector enables creation of 2D vectors
        if(isLeapYear(year)){
            for(int day=1;day<=366;day++){//This iterates through a full year
                yearlySum+=dataVector.at(row).at(4);
                row++;
            }
        thisYear.push_back(year);
        thisYear.push_back(yearlySum/366);
        averagesVector.push_back(thisYear);
        }
        else{
            for(int day=1;day<=365;day++){//This iterates through a full year
                yearlySum+=dataVector.at(row).at(4);
                row++;
            }
        thisYear.push_back(year);
        thisYear.push_back(yearlySum/365);
        averagesVector.push_back(thisYear);
        }
    }
}

float totalAverage(vector <vector<float>> &averagesVector){
    float totalSum=0;
    for(size_t i = 0; i < averagesVector.size(); ++i){
        totalSum+=averagesVector.at(i).at(1);
    }
    return totalSum/averagesVector.size();
}

/**
This function separates the averagesVector into new vectors containing yearly average values above and below the total average. These are later only used for plotting the histograms
*/
void separateData(const float totalMean,const vector<vector <float>> &averagesVector,vector <Double_t> &x_aroundMean,vector <Double_t> &y_aroundMean,vector <Double_t> &y_above,vector <Double_t> &y_below){
   Int_t n = averagesVector.size();
   for (Int_t i=0; i<n; i++) {
        x_aroundMean.push_back(averagesVector.at(i).at(0));//This vector is not necessary but is used for simplicity
        y_aroundMean.push_back(averagesVector.at(i).at(1)-totalMean);
        if(y_aroundMean[i]<0){//Split values into respective above or below average vector
            y_below.push_back(y_aroundMean[i]);
            y_above.push_back(0);//This is here to avoid undefined points and to match size with the x-axis
        }
        if(y_aroundMean[i]>=0){
            y_above.push_back(y_aroundMean[i]);
            y_below.push_back(0);
        }
   }

}
/**
This function takes the averagesVector and calcualtes the average yearly average for a set group of years. This set group of years is given it it's last argument.
@param:output vector, output vector Vector containing averages, the vector of values adjustefd for their position above or below the mean, the grouping size
*/
void makingMovingAverage(vector <Double_t> &y_movingAverage, vector <Double_t> &x_movingAverage, const vector<vector <float>> &averagesVector, vector <Double_t> &y_aroundMean,int groupSize){
    int yearCounter=0;//Keeps track of which year we are on. Dependent of number of groups
    int groupCounter=0;//Keeps track of which group we are on. i.e how many groups ahve been calculated
    double sum=0;
    int initialYear=1723;//Hardcoded!
    for(int i=0;i<(int)averagesVector.size();i++){
        yearCounter+=1;
        sum+=y_aroundMean[i];
        if(yearCounter==groupSize){
            y_movingAverage.push_back(sum/groupSize);
            x_movingAverage.push_back(initialYear+(groupCounter*groupSize)+(groupSize/2));
            yearCounter=0;//Reset year counter
            groupCounter+=1;
            sum=0;
            }
        }
        //Here the years that are left when the whole range is not dividible by the group size are taken care of
        y_movingAverage.push_back(sum/yearCounter);
        x_movingAverage.push_back(initialYear+(groupCounter*groupSize)+(yearCounter/2));
        
}

float tempTrender::tempEx(int year){

    //Set variables
    int startYear=1722;
    int endYear=2013;
    int groupSize1=30;
    int groupSize2=10;

    vector <vector <float>> dataVector;
    //This puts the raw data into a 2D vector (dataVector)
    readAllData("../datasets/uppsala_tm_1722-2013.dat",dataVector);

    vector <vector<float>> averagesVector;
    //This creates a vector (averagesVector) of average temperatures for each year from dataVector
    averages(dataVector,averagesVector);

    //Calculates the total average over the whole data set so that it is avaliabe later for plotting and extrapolation
    float totalMean=totalAverage(averagesVector);
    cout<<"The total average is: "<<totalMean<<endl;//Called functions such as tempEx should not have print statements. However this is necessary because the histograms are dependent on the relationship to the total average

    vector <Double_t> x_aroundMean,y_aroundMean,y_above,y_below;
    //This separates the averagesVector into new vectors containing values above and below the total average. These are only used for plotting the histograms
    separateData(totalMean,averagesVector,x_aroundMean,y_aroundMean,y_above,y_below);

    vector <Double_t> y_movingAverage1,x_movingAverage1;
    //This takes the averagesVector and calcualtes the average yearly average for a set group of years given as the last parameter
    makingMovingAverage(y_movingAverage1, x_movingAverage1, averagesVector, y_aroundMean,groupSize1);
    
    //Creates a graph object for the first moving average
    TGraph *gr_average1 = new TGraph (x_movingAverage1.size(), &x_movingAverage1[0], &y_movingAverage1[0]);

    vector <Double_t> y_movingAverage2,x_movingAverage2;
    makingMovingAverage(y_movingAverage2, x_movingAverage2, averagesVector, y_aroundMean,groupSize2);
    //Creates a graph object for the second moving average
    TGraph *gr_average2 = new TGraph (x_movingAverage2.size(), &x_movingAverage2[0], &y_movingAverage2[0]);

    //Creates graph objects of distribution of temperatures above and below the total average
    TGraph *gr_above = new TGraph (x_aroundMean.size(), &x_aroundMean[0], &y_above[0]);
    TGraph *gr_below = new TGraph (x_aroundMean.size(), &x_aroundMean[0], &y_below[0]);

    TCanvas * c2= new TCanvas("c2", "Yearly Temperature Averages",1200,600);
    c2->DrawFrame(startYear,-3,endYear,3);
    TMultiGraph *mg = new TMultiGraph();

    mg->GetXaxis()->SetTitle("Year");
    mg->GetYaxis()->SetTitle("Temperature (C),\n around given average");

    gr_above->SetFillColor(kRed+1);
    gr_below->SetFillColor(kBlue-4);
    mg->Add(gr_above,"B");//Since the y-axis is not integer the graphs represent histograms using the "B" plot option
    mg->Add(gr_below,"B");

    gr_average1->SetLineWidth(4);
    gr_average2->SetLineWidth(2);
    mg->Add(gr_average1,"C");
    mg->Add(gr_average2,"C");

    mg->Draw();
    
    c2->SaveAs("extrapolatedDataNoFit.jpg");//Sves a plot clean wihtout a fit

    TF1* fitFunc = new TF1("fitFunc", "([0]*(x-1840)*cos([1]*x))", startYear, endYear);//Creates a fit function in relation to the industrial revolution 1840

    fitFunc->SetParameter(0, 0.6);
    fitFunc->SetParameter(1, 0.125);

    fitFunc->SetLineColor(kGreen-3);
    fitFunc->SetLineWidth(3);
    gr_average1->Fit(fitFunc);
    
    //Create legend
    TLegend* leg = new TLegend(0.25,0.75,0.55,0.95);
    leg->SetFillStyle(0); //Hollow fill (transparent)
    leg->SetBorderSize(0); //Get rid of the border
    leg->SetNColumns(2);
    leg->AddEntry(gr_average1,"Average, group size 30","f");//Hardcoded, groupsize value
    leg->AddEntry(gr_average2,"Average, group size 10","f");//Hardcoded, groupsize value
    leg->AddEntry(gr_above,"Above","f");
    leg->AddEntry(gr_below,"Below","f");
    leg->AddEntry(fitFunc, "fit", "l");
    leg->Draw();

    mg->Draw();

    //Enable a return of a value from the function determined by the fit
    float param0=fitFunc->GetParameter(0);
    float param1=fitFunc->GetParameter(1);
    c2->SaveAs("extrapolatedDataFit.jpg");

    return param0*(year-1840)*cos(param1*year)+totalMean;//

}
