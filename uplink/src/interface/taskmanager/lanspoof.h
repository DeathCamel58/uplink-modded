
/*

  Lan Spoof

	Runs on a LAN viewer
	Targets a system on the LAN
	Spoofs the LAN into believing you are the system you are spoofing

  */

// ============================================================================

#ifndef _included_lanspoof_h
#define _included_lanspoof_h

#include "eclipse.h"
#include "interface/taskmanager/uplinktask.h"

#define LANSPOOF_UNUSED			0
#define LANSPOOF_SPOOFING		1
#define LANSPOOF_FAILED			2
#define LANSPOOF_SPOOFED		3

class LanComputer;

// ============================================================================



class LanSpoof : public UplinkTask
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

	LanSpoof();
	~LanSpoof() override;

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

