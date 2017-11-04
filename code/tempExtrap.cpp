#include <iostream>
#include "tempTrender.h"
#include <vector>
// ROOT library obejcts
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <fstream>
#include <TRandom3.h>

using namespace std;

void tempTrender::tempEx(){
    vector <vector <string> > data;
    readData("smhi-opendata_Lund.csv", data);
	//for(size_t i = 0; i < dataOnDay.size(); ++i) cout<<dataOnDay.at(i)<<endl;
	cout<<data.at(1).at(6)<<endl;

/*
	gRandom=new TRandom3();
        // create a histogram 
        TH1D* hist = new TH1D("data", ";x;N", 20, 0.0, 100.0);

       // fill in the histogram
        for (int i = 0; i < 100; ++i)
                hist->Fill(gRandom->Gaus(65.0, 5.0));

        TCanvas * c1= new TCanvas("c1", "random",5,5,800,600);
        hist->Draw();

	    vector <vector <string> > dataOnDay;
    readData("smhi-opendata_Lund.csv", dataOnDay);
*/
}
