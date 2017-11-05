#include <iostream>
#include "tempTrender.h"
// ROOT library obejcts
#include <TF1.h> // 1d function class
#include <TH1.h> // 1d histogram classes
#include <TStyle.h>  // style object
#include <TMath.h>   // math functions
#include <TCanvas.h> // canvas object
#include <fstream>
#include <TRandom3.h>
void tempTrender::tempEx(){
	gRandom=new TRandom3();
        // create a histogram 
        TH1D* hist = new TH1D("data", ";x;N", 20, 0.0, 100.0);

       // fill in the histogram
        for (int i = 0; i < 100; ++i)
                hist->Fill(gRandom->Gaus(65.0, 5.0));

        TCanvas * c1= new TCanvas("c1", "random",5,5,800,600);
        hist->Draw();
}
