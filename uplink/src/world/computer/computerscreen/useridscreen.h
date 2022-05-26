
/*
	
	Class UserIDScreen

	A computer screen which allows the user to id himself

	*/


#ifndef included_useridscreen_h
#define included_useridscreen_h

#include "world/computer/computerscreen/computerscreen.h"


class UserIDScreen : public ComputerScreen
{

public:

	int nextpage;
	int difficulty;									// 1 = easy, >1 = getting harder
	
public:

	UserIDScreen ();
	~UserIDScreen () override;

	void SetNextPage ( int newnextpage );
	void SetDifficulty ( int newdifficulty );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif

