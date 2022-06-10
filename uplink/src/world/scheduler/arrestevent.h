
/*

  Arrest event 

	Occurs when a person is arrested by federal agents
	(usually for hacking)

  */

#ifndef included_arrestevent_h
#define included_arrestevent_h


#include "world/person.h"
#include "world/scheduler/uplinkevent.h"


class ArrestEvent : public UplinkEvent
{

protected:

	string reason;
	char *ip;							

public:

	string name;

public:

	ArrestEvent ();
	~ArrestEvent () override;

	void SetName	(const string &newname );
	void SetReason	(const string &newreason );
	void SetIP		(const string &newip );

	void Run () override;

	string GetShortString () override;
	string GetLongString () override;

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};



#endif

