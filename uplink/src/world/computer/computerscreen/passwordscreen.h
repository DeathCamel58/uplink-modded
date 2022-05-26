
/*

	Password Screen definition

	Requires the correct password to pass

  */

#ifndef included_passwordscreen_h
#define included_passwordscreen_h

// ============================================================================

#include "world/computer/computerscreen/computerscreen.h"

#define SIZE_PASSWORDSCREEN_PASSWORD  64

// ============================================================================


class PasswordScreen : public ComputerScreen  
{

public:

	int nextpage;
	int difficulty;									// 1 = easy, >1 = getting harder
	char password [SIZE_PASSWORDSCREEN_PASSWORD];

public:

	PasswordScreen();
	~PasswordScreen() override;

	
	void SetNextPage ( int newnextpage );
	void SetPassword (const string& newpassword );
	void SetDifficulty ( int newdifficulty );


	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif
