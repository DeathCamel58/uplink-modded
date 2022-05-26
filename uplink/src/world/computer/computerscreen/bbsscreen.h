
/*

  BBS Screen class object

	Acts as a repository for messages which can be posted by anyone.
	
	Currently hacked to view world->missions only

  */

#ifndef included_bbsscreen_h
#define included_bbsscreen_h


#include "world/computer/computerscreen/computerscreen.h"


class BBSScreen : public ComputerScreen  
{

public:

	int nextpage;
	int contactpage;

public:

	BBSScreen();
	~BBSScreen() override;

	void SetNextPage ( int newnextpage );
	void SetContactPage ( int newcontactpage );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};

#endif
