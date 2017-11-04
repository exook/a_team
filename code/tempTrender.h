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
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

	private:
    string path;
    void readData(string fileName, vector <vector <string> > &data);
    
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

#endif
