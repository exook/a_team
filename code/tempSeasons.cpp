#include <iostream>
#include "tempTrender.h"

//calculate average temperature on each day
void calcAverageTemp(const vector <vector <string> > &data, vector <vector <double> > &averageTemp){
//    int year;
//    int month;
    int day;
    int yearPrevius = 0;
    int monthPrevius = 0;
    int dayPrevius = 0;
    
    double tempSum = 0;
    int counterTimes = 0;
    
    cout << int(data.size()) << endl;
    cout << int(data.at(0).size()) << endl;
    cout << int(data.at(1).size()) << endl;
    cout << int(data.at(2).size()) << endl;
    
    //loop through lines in the data
    for (int i=0; i < int(data.size()); i++)
    {
//        year = int(data.at(i).at(0));
//        month = int(data.at(i).at(1));
        day = stoi(data.at(i).at(2)); //stoi converts str to int
        
        vector<double> outputLine;
    
        if (day == dayPrevius){
            //sum temperaturs of the same date
            tempSum += stod(data.at(i).at(6));
            counterTimes++;
        }
        else{
            //save data in vector
            outputLine.push_back(yearPrevius);
            outputLine.push_back(monthPrevius);
            outputLine.push_back(dayPrevius);
            //calc average temperature of a date and save to vector
            outputLine.push_back(tempSum/counterTimes);
            
            //start temperature summation for new date
            tempSum = stod(data.at(i).at(6)); //stod converts str to double
            counterTimes = 1;
        }
        
        //save date and average temperature in vector
        averageTemp.push_back(outputLine);
        
        yearPrevius = stoi(data.at(i).at(0));
        monthPrevius = stoi(data.at(i).at(1));
        dayPrevius = day;
    }
}

//creates day a season starts to year histograms for all seasons
void tempTrender::startDaySeasons(){
    cout << endl;
    cout << "Calculating on which day the seasons start each year" << endl;
    cout << endl;
    
    vector <vector <string> > dataSeasons;
    vector <vector <double> > averageTempDay;
    
    readData("smhi-opendata_Lund.csv", dataSeasons);
    
    //calcAverageTemp(dataSeasons, averageTempDay);
    
    print(dataSeasons, 1);
}

















