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

void readDataOld(vector<vector<float>>* dataVectorPointer){	
	ifstream file("/home/alexander/Desktop/a_team/datasets/uppsala_tm_1722-2013.dat");
	//check if opened correctly
    if (!file) {
        cout << "Error could not read data file" << endl;
    }
    else {
        cout << "Succesfully opened data file" << endl;
    }

    string value;

    string line;
    while (getline(file,line)){
        istringstream helpstring(line);
        vector <float> row;
        while(getline(helpstring,value,' ')){
            if (value.find_first_not_of(' ') != std::string::npos){
                float number=strtof(value.c_str(),NULL);
                row.push_back(number);
            }
        
        }
        dataVectorPointer->push_back(row);
    }
    file.close();
}

int isLeapYear(int year){
    if((year % 4 == 0 && year % 100 != 0) || ( year % 400 == 0)){
        return 1;
    }
    else{
        return 0;
    }
}

void averages(vector<vector<float>>* dataVectorPointer,vector <vector<float>>* averagesVectorPointer){


    //cout<<initialYear<<","<<endYear<<endl;

    ofstream writer;
    writer.open("bug.txt");
    
    int row=0;
    for(size_t i = 0; i < dataVectorPointer->size(); ++i){
        row=i-1;
        if(dataVectorPointer->at(i).at(1)==1 && dataVectorPointer->at(i).at(2)==1){
            break;
        }
        else{
            //cout<<"No first of january found"<<endl;
        }
    }

    int initialYear=dataVectorPointer->at(row).at(0);
    int endYear=dataVectorPointer->at(dataVectorPointer->size()-1).at(0);

    vector <float> thisYear;

    for(int year = initialYear+1; year <= endYear;year++){//warning: comparison between signed and unsigned integer expressions [-Wsign-compare
        float yearlySum=0;
        vector <float> thisYear;
        if(isLeapYear(year)){
            for(int day=1;day<=366;day++){
                yearlySum+=dataVectorPointer->at(row).at(4);
                row++;
                //writer<<year<<":"<<day<<endl;
                //writer<<dataVectorPointer->at(row).at(0)<<":"<<dataVectorPointer->at(row).at(1)<<":"<<dataVectorPointer->at(row).at(2)<<endl<<endl;
            }
        thisYear.push_back(year);
        thisYear.push_back(yearlySum/366);
        averagesVectorPointer->push_back(thisYear);
        }
        else{
            for(int day=1;day<=365;day++){
                yearlySum+=dataVectorPointer->at(row).at(4);
                row++;
                //writer<<year<<":"<<day<<endl;
                //writer<<dataVectorPointer->at(row).at(0)<<":"<<dataVectorPointer->at(row).at(1)<<":"<<dataVectorPointer->at(row).at(2)<<endl<<endl;
            }
        thisYear.push_back(year);
        thisYear.push_back(yearlySum/366);
        averagesVectorPointer->push_back(thisYear);
        }
    }

    writer.close();
}

float totalAverage(vector <vector<float>>* averagesVectorPointer){
    float totalSum=0;
    for(size_t i = 0; i < averagesVectorPointer->size(); ++i){
        totalSum+=averagesVectorPointer->at(i).at(1);
    }
    return totalSum/averagesVectorPointer->size();
    

}

void tempTrender::tempEx(){

    vector <vector <float>> dataVector;
    vector <vector <float>> *dataVectorPointer=&dataVector;
    
    readDataOld(dataVectorPointer);

    vector <vector<float>> averagesVector;
    vector <vector<float>> *averagesVectorPointer=&averagesVector; 

    averages(dataVectorPointer,averagesVectorPointer);

    float totalMean=totalAverage(averagesVectorPointer);

   Int_t n = averagesVector.size();
   Double_t x[n], y[n],y_above[n],y_below[n];
   for (Int_t i=0; i<n; i++) {
        x[i] = averagesVector.at(i).at(0);
        y[i] = averagesVector.at(i).at(1)-totalMean;
        if(y[i]<0){
            y_below[i]=y[i];
            y_above[i]=0;
        }
        if(y[i]>=0){
            y_above[i]=y[i];
            y_below[i]=0;
        }
   }

    //int groupSize=20;
    int groupSize=30;//they used 5
    vector <Double_t> y_movingAverage,x_movingAverage;

    int counter=0;
    int counter2=0;
    int counter3=0;
    double sum=0;
    int initialYear=1723;//Hardcoded!
    for(Int_t i=0;i<n;i++){
        counter+=1;
        //cout<<y[i]<<endl;
        sum+=y[i];
        //cout<<"sum: "<<sum<<endl;
        counter3++;
        if(counter==groupSize){
            y_movingAverage.push_back(sum/groupSize);
            x_movingAverage.push_back(initialYear+(counter2*groupSize)-(groupSize/2)+groupSize);
            counter=0;
            counter2+=1;
            cout<<"Average: "<<y_movingAverage[i/groupSize]<<endl;
            sum=0;
        }
    }
y_movingAverage.push_back(sum/groupSize);
x_movingAverage.push_back(initialYear+(counter2*groupSize)-(groupSize/2)+groupSize);


cout<<endl<<"Actual array: "<<endl<<endl;

    for(int i=0;i<=n/groupSize;i++){
        cout<<x_movingAverage[i]<<","<<y_movingAverage[i]<<endl;

    }
cout<<sum/groupSize<<endl;

int N = x_movingAverage.size();
float x_movingAverageArray[N];
float y_movingAverageArray[N];
for (int i=0 ; i<N ; i++)
{
  x_movingAverageArray[i]=x_movingAverage[i];
  y_movingAverageArray[i]=y_movingAverage[i];
}
TGraph *g = new TGraph(N,x,y);


    TGraph *gr_average = new TGraph (N, x_movingAverageArray, y_movingAverageArray);
    TGraph *gr_above = new TGraph (n, x, y_above);
    TGraph *gr_below = new TGraph (n, x, y_below);

    TCanvas * c2= new TCanvas("c2", "random",1200,600);
    c2->DrawFrame(1722,-3.0,2013,3.0);
TMultiGraph *mg = new TMultiGraph();

    gr_above->SetFillColor(kRed-3);
    //gr_above->Draw("B");
    
    gr_below->SetFillColor(kBlue-3);
    //gr_below->Draw("B");

    //gr_average->Draw("P");
    gr_average->SetLineWidth(3);
    gr_average->SetMarkerStyle(8);
    gr_average->SetMarkerSize(1.5);
    
    //gr_average->GetXaxis()->SetTitle("year");
    //gr_average->Draw("L");

    mg->Add(gr_above,"B");
    mg->Add(gr_below,"B");
    mg->Add(gr_average,"PC");

    mg->GetXaxis()->SetTitle("Year");
    mg->GetYaxis()->SetTitle("Temperature (C)");  

    mg->Draw();

    //2*cos((1/291)((110+104)/2)*x+a)*cos((1/291)((7)/2)*x+a); x from 1722 to 2013
    //TF1* fitFunc = new TF1("fitFunc", "[0]*cos((1/291)((110+104)/2)*x+1.05)*cos((1/291)((7)/2)*x+1.05)", 1722, 2013);



//working

    TF1* fitFunc = new TF1("fitFunc", "([0]*(x-1840)*cos([1]*x))", 1722, 2013);

    fitFunc->SetParameter(0, 0.6);
    fitFunc->SetParameter(1, 0.025);
//

    fitFunc->SetLineColor(kGreen-3);
    fitFunc->SetLineWidth(3);
    gr_average->Fit(fitFunc);
    
    TLegend* leg = new TLegend(0.25,0.8,0.45,0.9);
    leg->SetFillStyle(0); //Hollow fill (transparent)
    leg->SetBorderSize(0); //Get rid of the border
    leg->SetNColumns(2);
    //leg->SetHeader("The Legend Title");
    leg->AddEntry(gr_average,"Average","f");
    leg->AddEntry(gr_above,"Above","f");
    leg->AddEntry(gr_below,"Below","f");
    leg->AddEntry(fitFunc, "fit", "l");
    leg->Draw();

    double_t param0=fitFunc->GetParameter(0);
    double_t param1=fitFunc->GetParameter(1);
    int year=2250;
    cout<<param0*(year-1840)*cos(param1*year);
    
    

}
