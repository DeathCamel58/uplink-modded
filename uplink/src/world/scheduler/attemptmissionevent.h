
/*

  Attempt Mission Event

	Represents a NPC agent attempting one of his missions

  */

#ifndef included_attemptmissionevent_h
#define included_attemptmissionevent_h


#include "world/person.h"
#include "world/scheduler/uplinkevent.h"


class AttemptMissionEvent : public UplinkEvent
{

public:

	string agentname;

public:

	AttemptMissionEvent ();
	~AttemptMissionEvent () override;

	void Run () override;

	void SetAgentName (const string &newagentname );

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

