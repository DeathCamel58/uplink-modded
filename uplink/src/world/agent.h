
/*

  Agent class object

	Part of the World sub-system

  */


#ifndef included_agent_h
#define included_agent_h

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

	void SetHandle (const string &newhandle );

	void GiveMission ( Mission *mission );
	void CheckMissionDueDates ();						// Have any missions expired?

	void GiveLink	 (const string &newip );
	bool HasLink     (const string &newip );
	void RemoveLink  (const string &newip );

	void GiveCode    (const string &newip, const string &newcode );
	void GiveMessage ( Message *message ) override;
    int HasAccount  (const string &ip );                           // Returns access level or -1

	int  CreateNewAccount (const string &bankip, const string &accname, const string &password,
                           int balance, int loan ) override;			// Returns account number

	bool HasMissionLink (const string &newip );

	void AttemptMission ();								// Attempts mission 0
	void AttemptMission_StealFile ();
    void AttemptMission_DeleteFile ();
	void AttemptMission_ChangeAccount ();
	void AttemptMission_TraceUser ();
	void AttemptMission_RemoveComputer ();

	void EstablishConnection (const string &ip );				// Complexity based on skill

    static bool ParseAccessCode (const string &thecode, string &username, size_t usernamesize, string &password, size_t passwordsize );

	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;

	string GetID () override;
	int   GetOBJECTID () override;

};


#endif 
