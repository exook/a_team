#include <iostream>
#include "tempTrender.h"
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <TLegend.h> // Legend object
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
	else if (fileName == "uppsala_tm_1722-2013.dat")
		location = "Uppsala";
	else 
		location = "Unknown";
	return location;
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
    readData(fileName, datahotCold);
    //print<vector <vector <string> > >(datahotCold, 3000);
    // define variables
    int yearPrevious = stoi(datahotCold.at(0).at(0));
    int monthPrevious = stoi(datahotCold.at(0).at(1));
    int dayPrevious = stoi(datahotCold.at(0).at(2));
    double minTemp = stod(datahotCold.at(0).at(6));
    double maxTemp = stod(datahotCold.at(0).at(6));
    int year, maxTempMonth = 0, maxTempDay = 0, minTempMonth = 0, minTempDay = 0, maxTempDayOfYear, minTempDayOfYear;
    double temp;

    for (int i = 0; i < int(datahotCold.size()); i++) {
		year = stoi(datahotCold.at(i).at(0));
		temp = stod(datahotCold.at(i).at(6));
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
		else if (year > yearPrevious) {
			/*cout << "year === " << year << endl;
			cout << "yearPrevious === " << yearPrevious << endl;
			cout << "maxTemp ==== " << maxTemp << endl;
			cout << "minTemp ==== " << minTemp << endl;*/
			maxTempDayOfYear = getDayOfYear(yearPrevious, maxTempMonth, maxTempDay);
			minTempDayOfYear = getDayOfYear(yearPrevious, minTempMonth, minTempDay);
			warmestHist->Fill(maxTempDayOfYear);
			coldestHist->Fill(minTempDayOfYear);
			// set max and min temp to the first day of next year
			maxTemp = stod(datahotCold.at(i).at(6));
			minTemp = stod(datahotCold.at(i).at(6));
			/*cout << "test new max min temp" << endl;
			cout << "datahotCold.at("<<i<<").at(6) === " << datahotCold.at(i).at(6) << endl;
			cout << "stoi(datahotCold.at("<<i<<").at(6)) === " << stod(datahotCold.at(i).at(6)) << endl;
			cout << "maxTemp ~~~~ " << maxTemp << endl;
			cout << "minTemp ~~~~ " << minTemp << endl;*/
			yearPrevious = year;
		}
		
	}
	
    
	TF1* func = new TF1("Gaussian", Gaussian, 1, 366, 3);
	func->SetParameters(5, 200, 50); //Starting values for fitting
	warmestHist->Fit(func, "Q1R");
	func->SetParameters(5, 10, 50); //Starting values for fitting
	coldestHist->Fit(func, "Q1R");
	cout << "The mean is " << func->GetParameter(1) << endl;
	cout << "Its uncertainty is " << func->GetParError(1) << endl;
    TLegend* leg = new TLegend(0.65, 0.75, 0.92, 0.92, "empty", "NDC");
	leg->SetFillStyle(0); //Hollow fill (transparent)
	leg->SetBorderSize(0); //Get rid of the border
	leg->AddEntry(warmestHist, "", "F"); //Use object title, draw fill
	leg->AddEntry(coldestHist, "", "F"); //Use custom title
	warmestHist->Draw();
	coldestHist->Draw("SAME");; //Draw on top of the existing plot
	leg->Draw(); //Legends are automatically drawn with "SAME"
	
    
}


//add your functions :)

