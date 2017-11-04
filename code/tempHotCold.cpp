#include <iostream>
#include "tempTrender.h"
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
double Gaussian(double* x, double* par) { //A custom function
		return par[0]*exp(-0.5*(x[0]*x[0] - 2*x[0]*par[1] + par[1]*par[1])/(par[2]*par[2]));
}

// Determine the location of data
string determineLocation(string fileName) {
	string location;
	if (fileName == "smhi-openda_Karlstad.csv") 
		location = "Karlstad";
	else if (fileName == "smhi-opendata_Boras.csv") 
		location = "Boras";
	else if (fileName == "smhi-opendata_Falsterbo.csv")
		location = "Falsterbo";
	else if (fileName == "smhi-opendata_Falun.csv")
		location = "Falun";
	else if (fileName == "smhi-opendata_Lulea.csv")
		location = "Lulea";
	else if (fileName == "smhi-opendata_Lund.csv")
		location = "Lund";
	else if (fileName == "smhi-opendata_Soderarm.csv")
		location = "Soderarm";
	else if (fileName == "smhi-opendata_Umea.csv")
		location = "Umea";
	else if (fileName == "smhi-opendata_Visby.csv")
		location = "Visby";
	else 
		location = "Unknown";
	return location;
}

// Calculate day of year
int getDayOfYear(int year, int month, int day) {
	bool checkleapYear = false;
	int dayOfYear = 0;
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		checkleapYear = true;
	}
	int daysInMonthsNonLeap[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int daysInMonthsLeap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (checkleapYear == false) {
		for (int k = 1; k < month; k++) {
			dayOfYear = dayOfYear + daysInMonthsNonLeap[k-1];
		}
		dayOfYear = dayOfYear + day;
	}
	else {
		for (int j = 1; j < month; j++) {
			dayOfYear = dayOfYear + daysInMonthsLeap[j-1];
		}
		dayOfYear = dayOfYear + day;
	}
	return dayOfYear;			
}


void tempTrender::hotCold(string fileName){
    cout << endl;
    cout << "Analyzing the warmest and coldest day of each year" << endl;
    cout << endl;
    string location;
    
    // determine the location of data 
    location = determineLocation(fileName);
    
    // create canvas and histograms for warmest and coldest
    TCanvas* c1 = new TCanvas("c1", "hot cold", 900, 600);
    TH1D* warmestHist = new TH1D("warmestHist", Form("The warmest day of %s; Day of year; Entries", location.c_str()), 70, 0, 366);
    TH1D* coldestHist = new TH1D("coldestHist", Form("The coldest day of %s; Day of year; Entries", location.c_str()), 70, 0, 366);
    
    // loading data
    vector <vector <string> > datahotCold;
    readData("smhi-opendata_Lund.csv", datahotCold);
    print(datahotCold, 10);
    // define variables
    int yearPrevious = stoi(datahotCold.at(0).at(0));
    int monthPrevious = stoi(datahotCold.at(0).at(1));
    int dayPrevious = stoi(datahotCold.at(0).at(2));
    double minTemp = stod(datahotCold.at(0).at(6));
    double maxTemp = stod(datahotCold.at(0).at(6));
    int year, maxTempMonth, maxTempDay, minTempMonth, minTempDay, maxTempDayOfYear, minTempDayOfYear;
    double temp;
    

    for (int i = 0; i < datahotCold.size(); i++) {
		year = stoi(datahotCold.at(i).at(0));
		temp = stoi(datahotCold.at(i).at(6));
		if (year == yearPrevious) {
			if (temp >= maxTemp) {
				maxTemp = temp;
				maxTempDay = stoi(datahotCold.at(i).at(2));
				maxTempMonth = stoi(datahotCold.at(i).at(1));
			}
			else if (temp < minTemp) {
				minTemp = temp;
				minTempDay = stoi(datahotCold.at(i).at(2));
				minTempMonth = stoi(datahotCold.at(i).at(1));
			}
		}
		else if (year == yearPrevious + 1) {
			cout << "year === " << year << endl;
			cout << "yearPrevious === " << yearPrevious << endl;
			cout << "maxTemp ==== " << maxTemp << endl;
			cout << "minTemp ==== " << minTemp << endl;
			maxTempDayOfYear = getDayOfYear(yearPrevious, maxTempMonth, maxTempDay);
			minTempDayOfYear = getDayOfYear(yearPrevious, minTempMonth, minTempDay);
			warmestHist->Fill(maxTempDayOfYear);
			coldestHist->Fill(minTempDayOfYear);
			// set max and min temp to the first day of next year
			maxTemp = stoi(datahotCold.at(i).at(6));
			minTemp = stoi(datahotCold.at(i).at(6));
			yearPrevious = yearPrevious +1;
		}
		
	}
    warmestHist->Draw();
    coldestHist->Draw("SAME"); 
    /*TCanvas* c1 = new TCanvas("c1", "hot cold", 900, 600);
    TH1D* warmestHist = new TH1D("warmestHist", Form("The warmest day of %s; Day of year; Entries", location.c_str()), 366, 0, 366);
    TH1D* coldestHist = new TH1D("coldestHist", Form("The warmest day of %s; Day of year; Entries", location.c_str()), 366, 0, 366);
	TF1* func = new TF1("Gaussian", Gaussian, 1, 366, 3);
	func->SetParameters(5, 200, 50); //Starting values for fitting
	warmestHist->Fit(func, "QDR");
	coldestHist->Fit(func, "QDR");
	cout << "The mean is " << func->GetParameter(1) << endl;
	cout << "Its uncertainty is " << func->GetParError(1) << endl;
    TLegend* leg = new TLegend(0.65, 0.75, 0.92, 0.92, "empty", "brNDC");
    leg->
	leg->SetFillStyle(0); //Hollow fill (transparent)
	leg->SetBorderSize(0); //Get rid of the border
	leg->AddEntry(warmestHist, "", "F"); //Use object title, draw fill
	leg->AddEntry(coldestHist, "A title", "F"); //Use custom title
	warmestHist->Draw();
	coldestHist->Draw("SAME"); //Draw on top of the existing plot
	leg->Draw(); //Legends are automatically drawn with "SAME"
	*/
    
}
