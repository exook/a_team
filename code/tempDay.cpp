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

// Left to do: probability to observe a certain temperature

void tempTrender::tempOnDay(int monthToCalculate, int dayToCalculate){
    cout << endl;
    cout << "Calculating the temperature for a certain day in Lund" << endl;
    
    vector <vector <string> > dataOnDay;
    readData("smhi-opendata_Lund.csv", dataOnDay);
    
    
    vector <float> tempCalculatedDay; // Vector to store temperatures for the chosen day
    // Create histogram
    TH1F* histogram = new TH1F("histogram", "Temperature;Temperature [#circC];Entries", 200, -20, 40);
    
    
    // Find the right day, month and time in the data
    for (int i = 1 ; i < (int)dataOnDay.size(); i++ ) {
        if ( (stoi(dataOnDay.at(i).at(1)) == monthToCalculate) && (stoi(dataOnDay.at(i).at(2)) == dayToCalculate) && (dataOnDay.at(i).at(3) == "12") ) {
            tempCalculatedDay.push_back(strtof((dataOnDay.at(i).at(6)).c_str(),0)); // Vector with temperatures for chosen day
            
        }
    }
    for(Int_t i = 0; i < (int)tempCalculatedDay.size(); i++) {
        histogram->Fill(tempCalculatedDay[i]);
    }
    
    
    
    histogram->SetFillColor(kBlue - 6);
    double mean = histogram->GetMean(); //The mean of the distribution
    double stdev = histogram->GetRMS(); //The standard deviation
    TCanvas* canv = new TCanvas();
    histogram->Draw();

    histogram->Fit("gaus");
    
    // Save the canvas as a picture
    canv->SaveAs("tempOnDay.png");
    
    
}





