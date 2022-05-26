
/*
	
	Class CypherScreen

	A computer screen which asks the user
	to enter a great big number

	*/


#ifndef included_cypherscreen_h
#define included_cypherscreen_h

#include "world/computer/computerscreen/computerscreen.h"


class CypherScreen : public ComputerScreen
{

public:

	int nextpage;
	int difficulty;									// 1 = easy, >1 = getting harder
	
public:

	CypherScreen ();
	~CypherScreen () override;

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

