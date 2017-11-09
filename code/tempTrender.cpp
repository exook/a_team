#include <fstream>
#include <sstream>
#include "tempTrender.h"

//constructor
tempTrender::tempTrender(string filePath) {
    cout << endl;
    cout << "This program calculates trends in the temprature datasets." << endl;
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
    // check if the data is from Upssala
    if (fileName != "uppsala_tm_1722-2013.dat") {
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
	} // end of reading data files except for Upssala
    
    else {
		string year, month, day, observed_temp, temp_urban_effect, dataID; // dataId = 1 (Uppsala), 2 (Risinge), 3 (Betna), 4 (Linkoping), 5 (Stockholm), 6 (Interpolated)
		int nRow = 0;
		// fill fictitious time and airqualityto match it with other dataset
		string hour = "-1", minute = "-1", sec = "-1", airquality = "-N";
		// read in data. The structure of the data vector is:
		// year month day hour min sec observed_temp air_quality dataID temp_urban_effect
		// Note that hour, min, sec and air_quality are all fictitious in order to match the format of other data file
		while (dataFile >> year >> month >> day >> observed_temp >> temp_urban_effect >> dataID) {
			if (dataID == "1") {
				vector <string> record_of_row;
				record_of_row.push_back(year);
				record_of_row.push_back(month);
				record_of_row.push_back(day);
				record_of_row.push_back(hour);
				record_of_row.push_back(minute);
				record_of_row.push_back(sec);
				record_of_row.push_back(observed_temp);
				record_of_row.push_back(airquality);
				//record_of_row.push_back(dataID); // No need to include dataID. Only Uppsala included
				record_of_row.push_back(temp_urban_effect);
				data.push_back(record_of_row);
			}
		}
	}
    //close file 
    dataFile.close();
}


// Test leap year
bool tempTrender::testLeapYear(int year) {
    bool check = false;
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        check = true;
    }
    return check;
}

// Calculate day of year
int tempTrender::getDayOfYear(int year, int month, int day) {
    bool checkLeapYear = false;
    int dayOfYear = 0;
    checkLeapYear = testLeapYear(year);
    int daysInMonthsNonLeap[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int daysInMonthsLeap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (checkLeapYear == false) {
        for (int k = 1; k < month; k++) {
            dayOfYear = dayOfYear + daysInMonthsNonLeap[k-1];
        }
        dayOfYear = dayOfYear + day;
    }
    else {
        for (int j = 1; j < month; j++) {
            dayOfYear = dayOfYear + daysInMonthsLeap[j-1];
        }
        dayOfYear = dayOfYear + day;
    }
    return dayOfYear;
}
















