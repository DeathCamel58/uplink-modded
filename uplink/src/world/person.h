
/*

  Person class object
  Part of the World sub-system

	Defines a non-player-character in the Uplink World
	including his physical behavoir, attributes, skills etc.

	The player is also a Person.

  */


#ifndef included_person_h
#define included_person_h

// ============================================================================

#include "app/uplinkobject.h"

#include "world/rating.h"
#include "world/connection.h"

#define SIZE_PERSON_NAME	 128

#define SIZE_VLOCATION_IP  24

#define PERSON_STATUS_NONE			0
#define PERSON_STATUS_INJAIL		1
#define PERSON_STATUS_DEAD			2


class Message;
class VLocation;

// ============================================================================


class Person : public UplinkObject  
{

public:

	char name	   [SIZE_PERSON_NAME];
	int age;

	int photoindex;							// Which picture to use
    int voiceindex;                         // Which voice samples to use

	char localhost  [SIZE_VLOCATION_IP];
	char remotehost [SIZE_VLOCATION_IP];
	char phonenumber [SIZE_VLOCATION_IP];

	LList <Message *> messages;
	LList <char *> accounts;				// List of [IP ACCNO]
	int currentaccount;						// Account for all current transfers

	Connection connection;
	Rating rating;
	
	bool istargetable;						// Can I be targeted by Random missions?

private:

	int STATUS;								// Bit field - see above

public:

	Person ();
	~Person () override;

	void SetName	    (const string &newname );
	void SetAge	        ( int newage );
	void SetPhotoIndex  ( int newindex );
    void SetVoiceIndex  ( int newindex );
	void SetLocalHost   (const string &newip );
	void SetRemoteHost  (const string &newip );
	void SetPhoneNumber (const string &newphone );
	void SetStatus	    ( int newSTATUS );
	int  GetStatus	    ( ) const;

	virtual void GiveMessage ( Message *message );

	bool IsConnected ();								// true if localhost != remotehost
	Connection *GetConnection ();
	VLocation *GetLocalHost ();	
	VLocation *GetRemoteHost ();						

	virtual int  CreateNewAccount (const string &bankip, const string &accname, const string &password,
                                   int balance, int loan );						// Returns account number

	int  GetBalance ();																// Overall bank balance
	void ChangeBalance (int amount, const string &description );
	void SetCurrentAccount ( int index );

	void SetIsTargetable ( bool newvalue );

	bool HasMessageLink (const string &newip );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};

#endif 
