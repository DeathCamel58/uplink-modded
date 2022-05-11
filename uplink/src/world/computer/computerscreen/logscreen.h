
/*

  LogScreen class object

  Shows the logs that exist on this computer

  */


#ifndef _included_logscreen_h
#define _included_logscreen_h

// ============================================================================

#include "world/computer/computerscreen/computerscreen.h"

#define LOGSCREEN_TARGET_UNKNOWN			-1
#define LOGSCREEN_TARGET_ACCESSLOGS			0
#define LOGSCREEN_TARGET_ACCOUNTLOGS		1					// MUST be on a Bank Computer and logged in

class LogBank;

// ============================================================================


class LogScreen : public ComputerScreen  
{

public:

	int nextpage;
	int TARGET;

public:

	LogScreen();
	~LogScreen() override;

	void SetNextPage ( int newnextpage ); 
	void SetTARGET ( int newTARGET );

	LogBank *GetTargetLogBank () const;								// Asserts log bank exists

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif
