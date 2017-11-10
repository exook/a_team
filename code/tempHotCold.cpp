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
    TH1D* warmestHist = new TH1D("warmestHist", Form("The warmest day of %s; Day of year; Entries", location.c_str()), 100, 0, 366);
    //TH1D* warmestHist = new TH1D("warmestHist", Form("The warmest day of %s; Day of year; Entries", location.c_str()), 732, -216, 516);
    TH1D* coldestHistLeft = new TH1D("coldestHistLeft", Form("The coldest day of %s; Day of year; Entries", location.c_str()), 100, -166, 200);
    TH1D* coldestHistRight = new TH1D("coldestHistRight", Form("The coldest day of %s; Day of year; Entries", location.c_str()), 100, 200, 566);
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
			bool checkLeapYear = testLeapYear(yearPrevious);
			maxTempDayOfYear = getDayOfYear(yearPrevious, maxTempMonth, maxTempDay);
			minTempDayOfYear = getDayOfYear(yearPrevious, minTempMonth, minTempDay);
			// Fill hot and cold histogram
			warmestHist->Fill(maxTempDayOfYear);
			if (minTempDayOfYear > 200) {
				coldestHistLeft -> Fill(minTempDayOfYear-366);
				coldestHistRight -> Fill(minTempDayOfYear);
				}
			else {
				coldestHistLeft -> Fill(minTempDayOfYear);
				coldestHistRight -> Fill(minTempDayOfYear+366);
			}
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
	
    
	TF1* funcHot = new TF1("Gaussian", Gaussian, 1, 366, 3);
	TF1* funcCold = new TF1("Gaussian", Gaussian, -216, 516, 3);
	funcHot->SetParameters(5, 200, 50); //Starting values for fitting
	funcHot->SetLineColor(kRed);
	warmestHist->Fit(funcHot, "Q1R");
	funcCold->SetParameters(5, 376, 50); //Starting values for fitting
	funcCold->SetLineColor(kBlue);
	coldestHistRight->Fit(funcCold, "Q1R");
	funcCold->SetParameters(5, 10, 50); //Starting values for fitting
	funcCold->SetLineColor(kBlue);
	coldestHistLeft->Fit(funcCold, "Q1R");
	
	
	cout << "The mean is for warmest days is " << funcHot->GetParameter(1) << endl;
	cout << "Its uncertainty (warmest day) is " << funcHot->GetParError(1) << endl;
	cout << "The mean is for coldest days is " << funcCold->GetParameter(1) << endl;
	cout << "Its uncertainty (coldest day) is " << funcCold->GetParError(1) << endl;
    TLegend* leg = new TLegend(0.6, 0.75, 0.92, 0.92, "", "NDC");
	leg->SetFillStyle(0); //Hollow fill (transparent)
	leg->SetBorderSize(0); //Get rid of the border
	leg->SetTextSize(0.035);
	leg->AddEntry(warmestHist, "", "F"); //Use object title, draw fill
	leg->AddEntry(coldestHistLeft, "", "F"); //Use custom title
	leg->AddEntry(funcHot, "Fit for warmest day", "L");
	leg->AddEntry(funcCold, "Fit for coldest day", "L");
	// Plot histograms and fit
	warmestHist->SetFillColor(kRed);
	warmestHist->SetFillStyle(3003);
	warmestHist->Draw();
	
	coldestHistLeft->SetFillColor(kBlue);
	coldestHistLeft->SetFillStyle(3003);
	coldestHistLeft->Draw("SAME"); //Draw on top of the existing plot
	coldestHistRight->SetFillColor(kBlue);
	coldestHistRight->SetFillStyle(3003);
	coldestHistRight->Draw("SAME");
	leg->Draw(); //Legends are automatically drawn with "SAME"
	
    c1->SaveAs(Form("HotCold%s.png", location.c_str()));
    c1->Close();
    delete warmestHist;
    delete coldestHistLeft;
    delete coldestHistRight;
}
