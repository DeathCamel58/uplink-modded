
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

	char *reason;
	char *ip;							

public:

	char name [SIZE_PERSON_NAME];

public:

	ArrestEvent ();
	~ArrestEvent () override;

	void SetName	(const string &newname );
	void SetReason	(const string &newreason );
	void SetIP		(const string &newip );

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

