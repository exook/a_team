#include <iostream>
#include <fstream>
#include <sstream>
#include "tempTrender.h"

//constructor
tempTrender::tempTrender(string filePath) {
    cout << endl;
    cout << "This program calculates trends in the temparture datasets." << endl;
    cout << "The path to the data files: " << endl;
    cout << filePath << endl;
    path = filePath;
}

//read in data from file into 2D vector
void tempTrender::readData(string fileName, vector <vector <string> > &data) {
    //open file
    ifstream dataFile(path + fileName);
    
    //check if opened correctly
    if (!dataFile) {
        cout << "Error could not read data file" << endl;
    }
    else {
        cout << "Succesfully opened data file" << endl;
    }
    
    string line;
    bool save = false; //to skip the first lines
    bool start = false;
    
    //read in data
    while (getline(dataFile, line)) {
        // skip empty lines
        if(!line.empty()) {
            istringstream helpstring(line);
            vector <string> record_of_row;
            // break a row into different different parts based on the data structure
            while (helpstring) {
                string data_of_row;
                
                //find the end of the header lines
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
            if (save && start) {
                data.push_back(record_of_row);
            }
            else if (save) {
                start = true;
            }
        }
    }
    
    if (!dataFile.eof()) {
        cerr<< "Error reading the file!\n";
    }
    
    //close file 
    dataFile.close();
}

//prints data in vector for the first given number of lines
void tempTrender::print(const vector <vector <string> > vec, int lines){
    cout << endl;
    
    //loop through elements in the vector and print them
    for (int i=0; i < lines; i++)
    {
        for (int j = 0; j < int(vec.at(i).size()); j++)
        {
            cout << vec.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

















