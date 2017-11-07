#include <iostream>
#include "tempTrender.h"
#include <sstream> // maybe include in tempTrender.h instead
// ROOT library objects
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <TGraph.h>
#include <TLegend.h>
// Left to do: probability to observe a certain temperature
double Gaussian2(double* x, double* par) { //A custom function
    return par[0]*exp(-0.5*(x[0]*x[0] - 2*x[0]*par[1] + par[1]*par[1])/(par[2]*par[2]));
}

void tempTrender::tempOnDay(int monthToCalculate, int dayToCalculate){
    cout << endl;
    cout << "Calculating the temperature for a certain date in Lund" << endl;
    
    vector <vector <string> > dataOnDay;
    readData("smhi-opendata_Lund.csv", dataOnDay);
    
    
    vector <float> tempCalculatedDay; // Vector to store temperatures for the chosen day
    // Create histogram
    TH1F* histogram = new TH1F("histogram", "Temperature;Temperature [#circC];Entries", 300, -20, 40);
    
    
    // Find the right day, month and time in the data
    for (int i = 1 ; i < (int)dataOnDay.size(); i++ ) {
        if ( (stoi(dataOnDay.at(i).at(1)) == monthToCalculate) && (stoi(dataOnDay.at(i).at(2)) == dayToCalculate) && (dataOnDay.at(i).at(3) == "12") ) {
            tempCalculatedDay.push_back(strtof((dataOnDay.at(i).at(6)).c_str(),0)); // Vector with temperatures for chosen day
            
        }
    }
    // Fill the histogram
    for(Int_t i = 0; i < (int)tempCalculatedDay.size(); i++) {
        histogram->Fill(tempCalculatedDay[i]);
    }
    
    //gStyle->SetOptStat(1111);
    
    histogram->SetFillColor(kRed + 1);
    double mean = histogram->GetMean(); //The mean of the distribution
    double stdev = histogram->GetRMS(); //The standard deviation
    TCanvas* c1 = new TCanvas("c1", "Date");
    histogram->Draw();
    
    
    TF1* func = new TF1("Gaussian", Gaussian2, -20, 40, 3);
    func->SetParameters(1, 5, 3); //Starting values for fitting
    func->SetLineColor(kBlack);
    
    histogram->Fit(func, "Q1R");
    
    func->GetParameter(0); //The constant
    double fitmean = func->GetParameter(1);//The mean
    double fitstdev = func->GetParameter(2);//The standard deviation
    double errormean = func->GetParError(1); //Error of parameter
    double errorstdev = func->GetParError(2);
    
    TLegend* legen = new TLegend(0.2,0.7,0.5,0.9);
    legen->SetFillStyle(0); //Hollow fill (transparent)
    legen->SetBorderSize(0); //Get rid of the border
    //leg->SetHeader("The Legend Title");
    legen->AddEntry(histogram,"Temperature on 19/7","f");
    legen->AddEntry(func, "Gaussian fit", "l");
    legen->Draw();
    
    // Save the canvas as a picture
    c1->SaveAs("tempOnDay.png");
    
    cout << "Data mean: "<<mean << ", fitted function mean: " << fitmean<< " with error: " << errormean<< endl;
    cout << "Data standard deviation: "<<stdev << ", fitted function standard deviation: " << fitstdev<< " with error: " << errorstdev<< endl;
    
}



