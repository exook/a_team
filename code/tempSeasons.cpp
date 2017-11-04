#include <iostream>
#include "tempTrender.h"

//prints data in vector for the first given number of lines
//TODO: merge print vectors
void printDoubleVector(const vector <vector <double> > vec, int lines){
    
    for (int i=0; i < lines; i++)
    {
        for (int j = 0; j < int(vec.at(i).size()); j++)
        {
            cout << vec.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

//prints data in vector for the first given number of lines
void printIntVector(const vector <vector <int> > vec, int lines){
    
    for (int i=0; i < lines; i++)
    {
        for (int j = 0; j < int(vec.at(i).size()); j++)
        {
            cout << vec.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

//calculate average temperature on each day
void calcAverageTemp(const vector <vector <string> > &data, vector <vector <double> > &averageTemp){

    int day;
    
    //load data first day
    int yearPrevius = stoi(data.at(0).at(0));
    int monthPrevius = stoi(data.at(0).at(1));
    int dayPrevius = stoi(data.at(0).at(2));
    
    double tempSum = stod(data.at(0).at(6));
    int counterTimes = 1;
    
    //loop through lines in the data
    for (int i=1; i < int(data.size()); i++)
    {
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
            
            //save date and average temperature in vector
            averageTemp.push_back(outputLine);
            
            //start temperature summation for new date
            tempSum = stod(data.at(i).at(6)); //stod converts str to double
            counterTimes = 1;
        }
        
        yearPrevius = stoi(data.at(i).at(0));
        monthPrevius = stoi(data.at(i).at(1));
        dayPrevius = day;
    }
}

//finds the first day of each winter and saves the date in a vector
void beginningWinter(const vector <vector <double> > &averageTemp, vector <vector <int> > &beginDayWinter){
    
    int yearPrevius = averageTemp.at(0).at(0)-1; //first year in data -1
    int monthPrevius = 12; //assume the winter has started in the previous year (before data)
    int currentYear;
    int currentMonth;
    int yearFirst;
    int monthFirst;
    int dayFirst;
    int counterDays = 0;
    
    //loop through dates in the vector
    for (int i=1; i < int(averageTemp.size()); i++){
        
        //is it a wintertemperature
        if (averageTemp.at(i).at(3) <= 0){
            //remember date of first winter day
            if (counterDays == 0) {
                yearFirst = averageTemp.at(i).at(0);
                monthFirst = averageTemp.at(i).at(1);
                dayFirst = averageTemp.at(i).at(2);
            }
            counterDays++;
            
            currentYear = averageTemp.at(i).at(0);
            currentMonth = averageTemp.at(i).at(1);
            
            //The winter does not always start once per year (month 1 to 12)
            //There are 4 cases of how the begin days of winters can follow
            
            //successive winters both early in year (month < 6)
            bool case1 = (currentYear != yearPrevius) &&
                         ((monthPrevius < 6) && (currentMonth < 6));
            //successive winters both late in year (month > 6)
            bool case2 = (currentYear != yearPrevius) &&
                         ((monthPrevius > 6) && (currentMonth > 6));
            //succesive winters are in the same year (one early the other late)
            bool case3 = (monthPrevius < 6) && (currentMonth > 6);
            //succesive winters skip a year (one winter late, the next early)
            bool case4 = (monthPrevius > 6) && (currentYear - yearPrevius == 2);
            
            //definition begining of a new winter
            if (counterDays == 5 && (case1 || case2 || case3 || case4)) {
                
                vector<int> outputLine;
                
                //save date first day winter of a year
                outputLine.push_back(yearFirst);
                outputLine.push_back(monthFirst);
                outputLine.push_back(dayFirst);
                
                beginDayWinter.push_back(outputLine);
                
                yearPrevius = averageTemp.at(i).at(0);
                monthPrevius = averageTemp.at(i).at(1);
            }
        }
        else {
            counterDays = 0;
        }
        
    }
}

template <class T>
int dateToInt (const T vec, int line) {
    if (int(vec.at(line).size()) >= 3) {
        string year = to_string(vec.at(line).at(0));
        string month = to_string(vec.at(line).at(1));
        string day = to_string(vec.at(line).at(2));

        int date = stoi(year + month + day);
        return date;
    }
    else {
        return 1;
    }
}

//finds the first day of spring for each year and saves the date in a vector
//TODO: not yet working
void beginningSpring(const vector <vector <double> > &averageTemp,
                     const vector <vector <int> > &beginDayWinter,
                     vector <vector <int> > &beginDaySpring){
    
    int yearPrevius = averageTemp.at(0).at(0)-1; //first year in data -1
    int yearFirst;
    int monthFirst;
    int dayFirst;
    int counterDays = 0;
    
    //loop through dates in the vector
    for (int i=0; i < int(averageTemp.size()); i++){
        
        //is it a springtemperature
        if (averageTemp.at(i).at(3) > 0 && averageTemp.at(i).at(3) < 10){
            //remember date of first spring day
            if (counterDays == 0) {
                yearFirst = averageTemp.at(i).at(0);
                monthFirst = averageTemp.at(i).at(1);
                dayFirst = averageTemp.at(i).at(2);
            }
            counterDays++;
            
            
            //definition begining of spring
            if (counterDays == 7) {
                
                //check if this spring date is after the start of winter
                bool afterWinter = true;
                int currentYear = averageTemp.at(i).at(0);
                
//                //TODO can be more efficient with while year below equal
//                //TODO not yet functional
//                for (int j=0; j < int(beginDayWinter.size()); j++){
//                    if ((currentYear == beginDayWinter.at(j).at(0)) &&
//                        (beginDayWinter.at(j).at(1) < 6)) {
//                        
//                        //get date of winter and spring to compare
//                        int winterDate = dateToInt<vector <vector <int> >>(beginDayWinter, j);
//                        int springDate = dateToInt<vector <vector <double> >>(averageTemp, i);
//                        
//                        if (springDate < winterDate) {
//                            afterWinter = false;
//                        }
//                    }
//                }
                
                if ((currentYear != yearPrevius) && afterWinter) {
                    vector<int> outputLine;
                    
                    //save date first day spring of a year
                    outputLine.push_back(yearFirst);
                    outputLine.push_back(monthFirst);
                    outputLine.push_back(dayFirst);
                    
                    beginDaySpring.push_back(outputLine);
                    
                    yearPrevius = currentYear;
                }
                else {
                    counterDays = 0;
                }
            }
        }
        else {
            counterDays = 0;
        }
        
    }
}


//creates day a season starts to year histograms for all seasons
void tempTrender::startDaySeasons(){
    cout << endl;
    cout << "Calculating on which day the seasons start each year" << endl;
    cout << endl;
    
    //https://www.smhi.se/kunskapsbanken/meteorologi/arstider-1.1082
    //winter t_average =< 0 for 5 days -> first of those days season begins
    //spring 0< t_average <10 for 7 days
    //summer t_average >= 10 for 5 days
    //fall 0< t_average <10 for 5 days
    
    
    vector <vector <string> > dataSeasons;
    vector <vector <double> > averageTempDay;
    vector <vector <int> > firstDayWinter;
    vector <vector <int> > firstDaySpring;
    
    readData("smhi-opendata_Lund.csv", dataSeasons);
    calcAverageTemp(dataSeasons, averageTempDay);
    beginningWinter(averageTempDay, firstDayWinter);
    beginningSpring(averageTempDay, firstDayWinter, firstDaySpring);
    
    int winterDate = dateToInt<vector <vector <int> >>(firstDayWinter, 0);
    cout << winterDate << endl;
    
    //print(dataSeasons, 5);
    //cout << endl << endl;
    //printDoubleVector(averageTempDay, 5);
    cout << endl << endl;
    printIntVector(firstDayWinter, 10);
    cout << endl << endl;
    cout << firstDayWinter.size() << endl;
    cout << firstDaySpring.size() << endl;
    cout << endl;
    printIntVector(firstDaySpring, 10);
    
}

















