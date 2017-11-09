#include <iostream>
#include "tempTrender.h"
#include <sstream> 
// ROOT library objects
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <TGraph.h>
#include <TLegend.h>

// The fit function
double Gaussian2(double* x, double* par) {
    return par[0]*exp(-0.5*(x[0]*x[0] - 2*x[0]*par[1] + par[1]*par[1])/(par[2]*par[2]));
}

void tempTrender::tempOnDay(int monthToCalculate, int dayToCalculate){
    cout << endl;
    cout << "Calculating the temperature for a certain date in Lund" << endl;
    
    // Read the data
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
    
    // Draw the histogram
    histogram->SetFillColor(kBlue);
    double mean = histogram->GetMean(); // The mean of the distribution
    double stdev = histogram->GetRMS(); // The standard deviation
    TCanvas* c1 = new TCanvas("c1", "Date");
    histogram->Draw();
    
    // Create a fit function
    TF1* func = new TF1("Gaussian", Gaussian2, -20, 40, 3);
    func->SetParameters(1, 5, 3); //Starting values for fitting
    func->SetLineColor(kBlack);
    histogram->Fit(func, "Q1R");
    
    // Parameters of the fit function
    func->GetParameter(0); // The constant
    double fitmean = func->GetParameter(1);// The mean of the fit function
    double fitstdev = func->GetParameter(2);// The standard deviation of the fit function
    double errormean = func->GetParError(1); // Error of parameters
    double errorstdev = func->GetParError(2);
    
    // Create a legend
    TLegend* legen = new TLegend(0.2,0.7,0.5,0.9);
    legen->SetFillStyle(0); // Transparent
    legen->SetBorderSize(0); // No border
    legen->AddEntry(histogram,"Temperature on 19/7","f");
    legen->AddEntry(func, "Gaussian fit", "l");
    legen->Draw();
    
    // Save the canvas as a picture
    c1->SaveAs("tempOnDay.png");
    
    cout << "Data mean: "<<mean << endl;
    cout << "Data standard deviation: "<<stdev<< endl;
    cout << "Number of data points: "<<tempCalculatedDay.size()<<endl; // To know how many data points

}



