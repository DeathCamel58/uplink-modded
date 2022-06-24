
/*

  Disconnected screen class object

	Shows a message to the player
    "You have been disconnected"
    "You lost the following accounts:"

  */


#ifndef included_disconnectedscreen_h
#define included_disconnectedscreen_h

// ============================================================================

#include <cstdio>

#include "world/computer/computerscreen/computerscreen.h"

class ComputerScreen;

// ============================================================================


class DisconnectedScreen : public ComputerScreen  
{

public:

	int nextpage;
	char *textmessage;

    static DArray <string> loginslost;

public:

	DisconnectedScreen();
	virtual ~DisconnectedScreen();

	void SetNextPage ( int newnextpage );
	void SetTextMessage (const string &newtextmessage );

    static void AddLoginLost (string &ip );
    static void ClearLoginsLost ();

    // Common functions

	bool Load  ( FILE *file );			
	void Save  ( FILE *file );			
	void Print ();						
	
	string GetID ();
	int   GetOBJECTID ();				

};

#endif
