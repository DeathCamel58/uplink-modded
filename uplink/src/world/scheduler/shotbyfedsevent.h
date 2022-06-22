
/*

  Shot By Feds event object

	Occurs when someone is shot by federal agents

  */


#ifndef included_shotbyfedsevent_h
#define included_shotbyfedsevent_h

// ============================================================================

#include "eclipse.h"

#include "world/person.h"
#include "world/scheduler/uplinkevent.h"

// ============================================================================



class ShotByFedsEvent : public UplinkEvent
{

protected:

	string reason;

public:

	string name;

public:

	ShotByFedsEvent ();
	~ShotByFedsEvent () override;

	void SetName (const string &newname );
	void SetReason (const string &newreason );

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
