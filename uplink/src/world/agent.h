
/*

  Agent class object

	Part of the World sub-system

  */


#ifndef _included_agent_h
#define _included_agent_h

// ============================================================================

#include <cstdio>

#include "world/person.h"
#include "world/computer/gateway.h"

class Message;
class Mission;

#define SIZE_AGENT_HANDLE		64


// ============================================================================


class Agent : public Person
{
	
public:

	LList <char *> links;					// The agents' "favourites" (IP)
	BTree <char *> codes;					// All known access codes, indexed on ip
	LList <Mission *> missions;				// The agents' missions	

	char handle [SIZE_AGENT_HANDLE];

public:

	Agent ();
	~Agent () override;

	void SetHandle ( char *newhandle );

	void GiveMission ( Mission *mission );
	void CheckMissionDueDates ();						// Have any missions expired?

	void GiveLink	 ( char *ip );
	bool HasLink     ( char *ip );
	void RemoveLink  ( char *ip );

	void GiveCode    ( char *newip, char *newcode );
	void GiveMessage ( Message *message ) override;
    int HasAccount  ( char *ip );                           // Returns access level or -1

	int  CreateNewAccount ( char *bankip, char *accname, char *password, 
							int balance, int loan ) override;			// Returns account number

	bool HasMissionLink ( const char *ip );

	void AttemptMission ();								// Attempts mission 0
	void AttemptMission_StealFile ();
    void AttemptMission_DeleteFile ();
	void AttemptMission_ChangeAccount ();
	void AttemptMission_TraceUser ();
	void AttemptMission_RemoveComputer ();

	void EstablishConnection ( char *ip );				// Complexity based on skill

    static bool ParseAccessCode ( const char *code, char *username, size_t usernamesize, char *password, size_t passwordsize );

	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;

	string GetID () override;
	int   GetOBJECTID () override;

};


#endif 
