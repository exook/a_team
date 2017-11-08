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
    bool testLeapYear(int year);
    template <class T> void print(T vec, int lines);
    
    void beginningWinter(const vector <vector <double> > &averageTemp,
                         vector <vector <int> > &beginDayWinter);
//    void beginningFall(const vector <vector <double> > &averageTemp,
//                       const vector <vector <int> > &beginDaySummer,
//                       vector <vector <int> > &beginDayFall);
//    void beginningSpring(const vector <vector <double> > &averageTemp,
//                         const vector <vector <int> > &beginDayWinter,
//                         vector <vector <int> > &beginDaySpring);
    void beginningSummer(const vector <vector <double> > &averageTemp,
                         vector <vector <int> > &beginDaySummer);
    void beginningSpringFall(const vector <vector <double> > &averageTemp,
                             const vector <vector <int> > &beginDayFirst,
                             vector <vector <int> > &beginDaySecond,
                             string season);
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

#endif
