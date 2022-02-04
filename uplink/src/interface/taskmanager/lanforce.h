
/*
  LAN Force software tool

	Forces a LAN lock open
    Wakes up the system admin

  */

#ifndef _included_lanforce_h
#define _included_lanforce_h

#include "eclipse.h"
#include "interface/taskmanager/uplinktask.h"


#define LANFORCE_UNUSED				0
#define LANFORCE_FORCING    		1
#define LANFORCE_FINISHED			3

class LanComputer;

// ============================================================================


class LanForce : public UplinkTask
{

protected:

	int status;
	int progress;
	int numticksrequired;

	LanComputer *comp;
	int systemIndex;

protected:

	static void BorderDraw    ( Button *button, bool highlighted, bool clicked );
	static void ProgressDraw  ( Button *button, bool highlighted, bool clicked );

	static void CloseClick    ( Button *button );
	static void BorderClick   ( Button *button );

public:

	LanForce();
	~LanForce() override;

	void SetTarget (UplinkObject *uo, const string &uos, int uoi ) override;
	void MoveTo ( int x, int y, int time_ms ) override;							// Origin top left

	void Initialise () override;
	void Tick ( int n ) override;
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};

#endif
