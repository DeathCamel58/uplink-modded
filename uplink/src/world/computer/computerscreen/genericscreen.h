
/*
	
		Generic Screen class

	This is used for screens which have no specific data and have no need
	of their own ComputerScreen class.

	For example, the FileServerScreen has no data - it simply lists the data on 
	the current computer.  This was previously accomplished with a virtually 
	empty FileServerScreen class.  This is now done with a GenericScreen class,
	with the type set to SCREEN_FILESERVERSCREEN.

  */


#ifndef included_genericscreen_h
#define included_genericscreen_h

#include "world/computer/computerscreen/computerscreen.h"


class GenericScreen : public ComputerScreen
{

public:

	int nextpage;
	int SCREEN_TYPE;

public:

	GenericScreen ();
	~GenericScreen () override;

	void SetNextPage ( int newnextpage ); 
	void SetScreenType ( int newSCREEN_TYPE );


	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};



#endif

