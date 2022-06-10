
/*

  Run Plot Scene Event

	Runs the next scene in the main plot line

  */


#ifndef included_runplotsceneevent_h
#define included_runplotsceneevent_h


#include "world/scheduler/uplinkevent.h"



class RunPlotSceneEvent : public UplinkEvent
{

public:

	int act;                                                    // If act = -1, this is a DemoPlotSceneEvent
	int scene;

public:

	RunPlotSceneEvent ();
	~RunPlotSceneEvent () override;

	void SetActAndScene ( int newact, int newscene );

	void Run () override;

	string GetShortString () override;
	string GetLongString () override;

	
	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif

