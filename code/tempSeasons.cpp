#include <iostream>
#include "tempTrender.h"

////calculate average temperature on each day
//void calcAverageTemp(const vector <vector <string> > &data, vector <vector <int> > &averageTemp){
//    string year;
//    string month;
//    string day;
//    
//    int tempSum;
//    int counterTimes;
//    
//    
//    //loop through elements in the vector and print them
//    for (int i=0; i < int(data.size()); i++)
//    {
//        year = data.at(i).at(0);
//        month = data.at(i).at(1);
//        day = data.at(i).at(2);
//        
//        for (int j = 0; j < int(data.at(i).size()); j++)
//        {
//            //cout << vec.at(i).at(j) << " ";
//        }
//        //cout << endl;
//    }
//}

//creates day a season starts to year histograms for all seasons
void tempTrender::startDaySeasons(){
    cout << endl;
    cout << "Calculating on which day the seasons start each year" << endl;
    
    vector <vector <string> > dataSeasons;
    //vector <vector <int> > averageTempDay;
    
    readData("smhi-opendata_Lund.csv", dataSeasons);
    
    print(dataSeasons, 10);
}

















