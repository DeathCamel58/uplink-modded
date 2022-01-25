
/*

  Arrest event 

	Occurs when a person is arrested by federal agents
	(usually for hacking)

  */

#ifndef _included_arrestevent_h
#define _included_arrestevent_h


#include "world/person.h"
#include "world/scheduler/uplinkevent.h"


class ArrestEvent : public UplinkEvent
{

protected:

	char *reason;
	char *ip;							

public:

	char name [SIZE_PERSON_NAME];

public:

	ArrestEvent ();
	~ArrestEvent () override;

	void SetName	( char *newname );
	void SetReason	( char *newreason );
	void SetIP		( char *newip );

	void Run () override;

	char *GetShortString () override;
	char *GetLongString () override;

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};



#endif

