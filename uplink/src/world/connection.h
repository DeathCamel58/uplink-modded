
/*

  Connection class object
	
	Represents a Person's connection from his physical location to his 
	ultimate virtual location.  

	Part of the World sub-system

  */
	
#ifndef _included_connection_h
#define _included_connection_h

// ============================================================================

#include "app/uplinkobject.h"

#include "world/date.h"

class Person;

// ============================================================================


class Connection : public UplinkObject  
{

public:

	char owner [64];
	LList <char *> vlocations;				// List of IP addresses, source to target order

	bool traceinprogress;
	int traceprogress;						// 0 = no progress, ... , [size of connection-1] = traced
	Date nexttrace;							// Date and time of next trace

public:
	
	Connection();
	~Connection() override;

	void SetOwner (const string &newowner );

	void AddVLocation (const string &ip );
	/********** Start code by François Gagné **********/
	void AddOrRemoveVLocation (const string &ip );   // If the ip is not currently in, add it else remove it
	/********** End code by François Gagné **********/
	void RemoveLastLocation ();				// Removes the most recently added
	
	bool LocationIncluded (const string &ip );

	Person *GetOwner ();
	char *GetSource ();						// Returns first ip (source machine) or NULL
	char *GetTarget ();						// Returns final ip (target machine) or NULL
	char *GetGhost ();						// Returns final - 1 ip (apparent source machine) or NULL
	int GetSize ();							// Returns number of nodes in connection

	void BeginTrace ();
	void EndTrace ();

	bool TraceInProgress ();
	bool Traced ();
	bool Connected ();

	void Connect ();
	void Disconnect ();
	void Reset ();

	
	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;

};

#endif 
