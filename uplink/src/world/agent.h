
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

//! Agent Person
/**
 * Agent Person
 * Part of the world sub-system
 */
class Agent : public Person
{
	
public:

    /**
     * The agent's "favorites" (IP)
     */
	LList <string> links;

	/**
	 * All known access codes, indexed on IP
	 */
	BTree <char *> codes;

	/**
	 * An LList that stores each Mission the Agent has
	 */
	LList <Mission *> missions;

	/**
	 * The name of the agent
	 */
	char handle [SIZE_AGENT_HANDLE];

public:

	Agent ();
	~Agent () override;

	/**
	 * Set the handle of the agent
	 * @param newhandle Handle to set
	 */
	void SetHandle (const string &newhandle );

	/**
	 * Gives a mission to the agent
	 * @param mission Mission to give
	 */
	void GiveMission ( Mission *mission );

	/**
	 * Check if any of the missions have expired
	 */
	void CheckMissionDueDates ();

	/**
	 * Gives a link to the agent
	 * @param newip IP of the link
	 */
	void GiveLink	 (const string &newip );

	/**
	 * Does the agent have a link?
	 * @param newip IP of the link
	 * @return true if the agent has the link
	 */
	bool HasLink     (const string &newip );

	/**
	 * Removes a link from the agent
	 * @param newip IP of the link
	 */
	void RemoveLink  (const string &newip );

	/**
	 * Give a code to the agent
	 * @param newip IP of the link
	 * @param newcode Code for the link
	 */
	void GiveCode    (const string &newip, const string &newcode );

    /**
     * Gives a message to the player
     * @param message Message to give
     */
	void GiveMessage ( Message *message ) override;

	/**
	 * Check if agent has an account for given link
	 * @param ip IP of the link
	 * @return access level or -1
	 */
    int HasAccount  (const string &ip );

    /**
     * Create a new account at a bank
     * @param bankip IP of the bank
     * @param accname Account name
     * @param password Account password
     * @param balance Account balance
     * @param loan Account loan amount
     * @return The account number
     */
	int  CreateNewAccount (const string &bankip, const string &accname, const string &password, int balance, int loan ) override;

	/**
	 * Checks if agent has given link from mission
	 * @param newip IP of the link
	 * @return true if the user has the link, and a mission that needs it
	 */
	bool HasMissionLink (const string &newip );

	/**
	 * Attempts mission 0
	 */
	void AttemptMission ();

	/**
	 * Attempts mission to steal a file
	 */
	void AttemptMission_StealFile ();

	/**
	 * Attempts mission to delete a file
	 */
    void AttemptMission_DeleteFile ();

    /**
     * Attempts mission to change account
     */
	void AttemptMission_ChangeAccount ();

	/**
	 * Attempts mission to trace a user
	 */
	void AttemptMission_TraceUser ();

	/**
	 * Attempts mission to remove a computer
	 */
	void AttemptMission_RemoveComputer ();

	/**
	 * Establishes a connection to IP
	 * @param ip IP of the link
	 * @note This generates a few random middle links in the connection, does some stuff, deletes logs, disconnects
	 * @note Higher uplink rating adds in more links
	 */
	void EstablishConnection (const string &ip );

    /**
     * Parses an access code to a username and password
     * Only 2 varieties are permitted:
     * 1.   ... 'name' .... 'code'
     * 2.   ... 'code' ....
     * In other words, names/codes are always enclosed in '
     * @param thecode The code to parse
     * @param username The username variable to populate
     * @param password The password variable to populate
     * @return `true` if it was successfully parsed
     */
    static bool ParseAccessCode(const string &thecode, string &username, string &password);

	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;

	string GetID () override;
	int   GetOBJECTID () override;

};


#endif 
