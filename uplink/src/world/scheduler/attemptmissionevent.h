
/*

  Attempt Mission Event

	Represents a NPC agent attempting one of his missions

  */

#ifndef _included_attemptmissionevent_h
#define _included_attemptmissionevent_h


#include "world/person.h"
#include "world/scheduler/uplinkevent.h"


class AttemptMissionEvent : public UplinkEvent
{

public:

	char agentname [SIZE_PERSON_NAME];

public:

	AttemptMissionEvent ();
	~AttemptMissionEvent () override;

	void Run () override;

	void SetAgentName (const string &newagentname );

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

