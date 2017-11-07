#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class tempTrender {
	public:
	tempTrender(string filePath); //Construct using the specified file
	~tempTrender() {} //Destructor
    
    void startDaySeasons();	
    void tempOnDay(int monthToCalculate, int dayToCalculate); //Make a histogram of the temperature on this date
	void tempOnDayNumber(int dateToCalculate); //Make a histogram of the temperature on this day
	//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	void hotCold(string fileName); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

	void tempEx();

	private:
    string path;
    void readData(string fileName, vector <vector <string> > &data);
    int getDayOfYear(int year, int month, int day);
    template <class T> void print(T vec, int lines);
};


//prints vectors of different types
//call as: print<type>(vec, lines);
//e.g. with type = vector <vector <string> >
template <class T>
void tempTrender::print(T vec, int lines) {
    if (int(vec.size()) >= lines) {
        //loop through vector and print
        for (int i=0; i < lines; i++)
        {
            for (int j = 0; j < int(vec.at(i).size()); j++)
            {
                cout << vec.at(i).at(j) << " ";
            }
            cout << endl;
        }
    }
}

// Calculate day of year
int tempTrender::getDayOfYear(int year, int month, int day) {
	bool checkleapYear = false;
	int dayOfYear = 0;
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		checkleapYear = true;
	}
	int daysInMonthsNonLeap[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int daysInMonthsLeap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (checkleapYear == false) {
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

#endif
