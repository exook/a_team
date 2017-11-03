#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

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
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year
	void tempPerYear(){
	}

	private:
    string path;
    void readData(string fileName, vector <vector <string> > &data);
    void print(const vector <vector <string> > vec,int lines);
};

#endif
