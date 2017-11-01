#include <iostream>
#include <fstream>
#include <sstream>
#include "tempTrender.h"

tempTrender::tempTrender(string filePath) {
    cout << endl;
    cout << "This program calculates trends in the temparture datasets." << endl;
    cout << "The path to the data files: " << endl;
    cout << filePath << endl;
    path = filePath;
}

void tempTrender::readData(string fileName, vector <vector <string> > &data) {
    
    ifstream dataFile(path + fileName);
    
    if (!dataFile) {
        cout << "Error could not read data file" << endl;
    }
    else {
        cout << "Succesfully opened data file" << endl;
    }
    
    string line;
    bool save = false; //to skip the first lines
    //read in data
    while (getline(dataFile, line)) {
        // skip empty lines
        if(!line.empty()) {
            istringstream helpstring(line);
            vector <string> record_of_row;
            // break a row into different different parts based on the data structure
            while (helpstring) {
                string data_of_row;
                
                if (!save) {
                    getline(helpstring, data_of_row, ';');
                    if (data_of_row == "Datum"){
                        save = true;
                        break;
                    }
                    break;
                }
                
                for (int ii = 0; ii<2; ii++) {
                    // break time into year month and day
                    if (!getline(helpstring, data_of_row, '-')) break;
                    record_of_row.push_back(data_of_row);
                }
                
                if (!getline(helpstring, data_of_row, ';')) break;
                record_of_row.push_back(data_of_row);
                
                for (int iii = 0; iii<2; iii++) {
                    if (!getline(helpstring, data_of_row, ':')) break;
                    record_of_row.push_back(data_of_row);
                }
                
                while (true) {
                    if (!getline(helpstring, data_of_row, ';')) break;
                    record_of_row.push_back(data_of_row);
                }
            }
            if (save) {
                data.push_back(record_of_row);
            }
        }
    }
    
    if (!dataFile.eof()) {
        cerr<< "Error reading the file!\n";
    }
    
    dataFile.close();
}

void tempTrender::print(const vector <vector <string> > vec){
    //prints the values in the vector
    cout << endl;
    
    //int lines = int(vec.size());
    int lines = 10;
    
    for (int i=0; i < lines; i++)
    {
        for (int j = 0; j < int(vec.at(i).size()); j++)
        {
            cout << vec.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

















