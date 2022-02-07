
/*

  Computer screen base object

	This should be derived by any computer screens
	such as passwordscreens, logscreens etc

	Note that you will have to provide the Common Functions
	(load, save, GetID etc) for each derived class

  */


#ifndef _included_computerscreen_h
#define _included_computerscreen_h

// ============================================================================

#include "app/uplinkobject.h"


#define SIZE_COMPUTERSCREEN_MAINTITLE 64
#define SIZE_COMPUTERSCREEN_SUBTITLE 64

#define SIZE_COMPUTER_NAME   64


class Computer;

// ============================================================================


class ComputerScreen : public UplinkObject  
{

public:

	char maintitle [SIZE_COMPUTERSCREEN_MAINTITLE];
	char subtitle  [SIZE_COMPUTERSCREEN_SUBTITLE];
	char computer  [SIZE_COMPUTER_NAME];

public:

	ComputerScreen();
	~ComputerScreen() override;


	void SetMainTitle (const string &newmaintitle );
	void SetSubTitle  (const string &newsubtitle );
	void SetComputer  (const string &newcomputer );

	Computer *GetComputer ();


	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;
	int GetOBJECTID () override;

};


#endif
