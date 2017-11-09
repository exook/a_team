#include "tempTrender.h"
#include <string>

void project() {
	string pathToFile = "../datasets/";
    tempTrender t(pathToFile); //Instantiate your analysis object
    
//    t.startDaySeasons();
//	t.tempEx();

//	t.tempOnDay(7, 19);
//	t.tempOnDayNumber(200);
	t.hotCold("uppsala_tm_1722-2013.dat");

}
