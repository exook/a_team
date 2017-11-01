#include <iostream>
#include "tempTrender.h"
#include <fstream>

tempTrender::tempTrender(string filePath) {
    cout << endl;
    cout << "This program calculates trends in the temparture datasets." << endl;
    cout << "The path to the data files: " << endl;
    cout << filePath << endl;
    path = filePath;
}

void tempTrender::openFile(string fileName){
    ifstream dataFile(path + fileName);
    
    if (!dataFile) {
        cout << "Error could not read data file" << endl;
        //return 0;
    }
    else {
        cout << "Succesfully opened data file" << endl;
    }
    
    dataFile.close();
}

void tempTrender::startDaySeasons(){
    cout << endl;
    cout << "Calculating on which day the seasons start each year" << endl;
    openFile("smhi-opendata_Lund.csv");
}