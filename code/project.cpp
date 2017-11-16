#include "tempTrender.h"
#include <string>

void project() {
	string pathToFile = "../datasets/";
    int year=2050;
    double temperature;
    
    tempTrender t(pathToFile); //Instantiate your analysis object
    
    t.startDaySeasons();
    temperature = t.tempEx(year);
    cout << endl << "The temperature in "<<year<<" will be: " << temperature << " degrees" << endl;
	t.tempOnDay(7, 19);
	t.tempOnDayNumber(198);
	t.hotCold("uppsala_tm_1722-2013.dat");
}
