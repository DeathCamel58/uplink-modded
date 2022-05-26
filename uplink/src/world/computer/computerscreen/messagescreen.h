
/*

  Message screen class object

	Shows a message and the company title

  */


#ifndef included_messagescreen_h
#define included_messagescreen_h

// ============================================================================

#include <cstdio>

#include "world/computer/computerscreen/computerscreen.h"

class ComputerScreen;

// ============================================================================


class MessageScreen : public ComputerScreen  
{

public:

	int nextpage;
	char *textmessage;
	char *buttonmessage;
	bool mailthistome;	

public:

	MessageScreen();
	~MessageScreen() override;

	void SetNextPage ( int newnextpage );
	void SetTextMessage (const string &newtextmessage );
	void SetButtonMessage (const string &newbuttonmessage );
	void SetMailThisToMe ( bool newvalue );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};

#endif
