
/*
	
	Links Screen

  */


#ifndef _included_linksscreen_h
#define _included_linksscreen_h

// ============================================================================

#include "world/computer/computerscreen/computerscreen.h"

#define  LINKSSCREENTYPE_NONE				0
#define  LINKSSCREENTYPE_ALLLINKS			1
#define  LINKSSCREENTYPE_PLAYERLINKS		2
#define  LINKSSCREENTYPE_LOCALLINKS			3

// ============================================================================


class LinksScreen : public ComputerScreen
{

public:

	int nextpage;
	int SCREENTYPE;

public:

	LinksScreen ();
	~LinksScreen () override;

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

