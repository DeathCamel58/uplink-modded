
/*

  Seize Gateway event

	Occurs when federal agents decide to grab your gateway
	(Only applies to Uplink Agents)

  */

#ifndef included_seizeevent_h
#define included_seizeevent_h


#include "world/person.h"
#include "world/scheduler/uplinkevent.h"


class SeizeGatewayEvent : public UplinkEvent
{

protected:

	char *reason;

public:

	char name [SIZE_PERSON_NAME];
	int gateway_id;							// id number of gateway to seize

public:

	SeizeGatewayEvent ();
	~SeizeGatewayEvent () override;

	void SetName (const string &newname );
	void SetReason (const string &newreason );
	void SetGatewayID ( int newid );

	void Run () override;
	void RunWarning () override;

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

