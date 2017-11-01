#include <iostream>
#include "tempTrender.h"

void tempTrender::startDaySeasons(){
    cout << endl;
    cout << "Calculating on which day the seasons start each year" << endl;
    
    vector <vector <string> > dataSeasons;
    readData("smhi-opendata_Lund.csv", dataSeasons);
    
    print(dataSeasons, 10);
}

















