
/*

  Shot By Feds event object

	Occurs when someone is shot by federal agents

  */


#ifndef _included_shotbyfedsevent_h
#define _included_shotbyfedsevent_h

// ============================================================================

#include "eclipse.h"

#include "world/person.h"
#include "world/scheduler/uplinkevent.h"

// ============================================================================



class ShotByFedsEvent : public UplinkEvent
{

protected:

	char *reason;

public:

	char name [SIZE_PERSON_NAME];

public:

	ShotByFedsEvent ();
	~ShotByFedsEvent () override;

	void SetName ( char *newname );
	void SetReason ( char *newreason );

	void Run () override;

	char *GetShortString () override;
	char *GetLongString () override;

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	char *GetID () override;
	int   GetOBJECTID () override;

};


#endif

