void rootlogon() {
    gStyle->SetOptStat(0); //Let's make our plots look a bit better. Get rid of the stat box
    gStyle->SetOptTitle(0); //Get rid of the title (good plots use legends instead)
    gStyle->SetTitleSize(0.05, "x"); //Use bigger text on the axes
    gStyle->SetTitleSize(0.05, "y");
    gStyle->SetLabelSize(0.05, "x"); //Use bigger labels too
    gStyle->SetLabelSize(0.05, "y");
    gStyle->SetPadTopMargin(0.05); //Change the margins to fit our new sizes
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadBottomMargin(0.16);
    gStyle->SetPadLeftMargin(0.16);

    //compile all files and run project()
    gROOT->ProcessLine(".L tempTrender.cpp+");
    gROOT->ProcessLine(".L tempSeasons.cpp+");
    gROOT->ProcessLine(".L tempDay.cpp+");
    gROOT->ProcessLine(".L tempDayNumber.cpp+");
    gROOT->ProcessLine(".L tempHotCold.cpp+");
    gROOT->ProcessLine(".L tempExtrap.cpp+");
    gROOT->ProcessLine(".L project.cpp+");
    gROOT->ProcessLine("project()");
}
