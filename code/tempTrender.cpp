#include <iostream>
#include "tempTrender.h"

tempTrender::tempTrender(string filePath) {
    cout << endl;
    cout << "This program calculates trends in the temparture datasets." << endl;
    cout << "The path to the data files: " << endl;
    cout << filePath << endl;
    path = filePath;
}

void tempTrender::startDaySeasons(){
    cout << endl;
    cout << "Calculating on which day the seasons start each year" << endl;
}