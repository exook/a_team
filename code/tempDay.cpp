#include <iostream>
#include "tempTrender.h"
#include <sstream> // maybe include in tempTrender.h instead
// ROOT library obejcts
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object

// lägga till att man kan välja tid på dygnet?
//add your functions :)

void tempTrender::tempOnDay(int monthToCalculate, int dayToCalculate){
    cout << endl;
    cout << "Calculating the temperature for a certain day in Lund" << endl;
    
    vector <vector <string> > dataOnDay;
    readData("smhi-opendata_Lund.csv", dataOnDay);
    
    
    vector <float> tempCalculatedDay; // Vector to store temperatures for the chosen day
    float temperatureOnDay; // Convert string to float for the chosen day
    // Create histogram
    TH1F* hist = new TH1F("hist", "Temperature;Temperature [#circC];Entries", 300, -20, 40);
    
    // DataOnDay is a string vector, input month and day are integers
    stringstream ss;
    ss << monthToCalculate;
    string month = ss.str(); // Converts month int to string, needed in for-loop condition
    while(month.length() < 2) { // Adds 0 in front of string if needed for comparison in for-loop condition
        month = "0" + month;
    }
    stringstream s;
    s << dayToCalculate;
    string day = s.str();
    while(day.length() < 2) {
        day = "0" + day;
    }
    
    // Find the right day in the data
    for (int i = 1 ; i < (int)dataOnDay.size(); i++ ) {
        if ( (dataOnDay.at(i).at(1) == month) && (dataOnDay.at(i).at(2) == day) && (dataOnDay.at(i).at(3) == "06") ) {
            temperatureOnDay = strtof((dataOnDay.at(i).at(6)).c_str(),0); // convert string to float
            tempCalculatedDay.push_back(temperatureOnDay); // Vector with temperatures for chosen day
            
        }
    }
    
    for(Int_t i = 0; i < (int)tempCalculatedDay.size(); i++) {
        hist->Fill(tempCalculatedDay[i]);
    }
    
    hist->SetFillColor(kRed + 1);
    double mean = hist->GetMean(); //The mean of the distribution
    double stdev = hist->GetRMS(); //The standard deviation
    TCanvas* canv = new TCanvas();
    hist->Draw();
    // Save the canvas as a picture
    canv->SaveAs("tempOnDay.png");
    
    //cout << mean << endl;
    //cout << stdev << endl;
    // Left to do: probability to observe a certain temperature
}





