
/*

  Revelation Tracker

	Sits on the players computer
    Informs him when computers are infected with Revelation

  */


#ifndef included_revelationtracker_h
#define included_revelationtracker_h



#include "interface/taskmanager/uplinktask.h"


class RevelationTracker : public UplinkTask
{

protected:

    time_t timesync;
    int height;                                             // In rows

protected:
    
    static void TitleClick ( Button *button );
    static void CloseClick ( Button *button );

    static void MainBoxDraw ( Button *button, bool highlighted, bool clicked );

public:

    RevelationTracker ();
    ~RevelationTracker () override;

    void MoveTo ( int x, int y, int time_ms ) override;

	void Initialise () override;
	void Tick ( int n ) override;

	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};



#endif

