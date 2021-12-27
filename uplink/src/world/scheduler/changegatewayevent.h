

/*

  Change Gateway Event

	When this occurs, the player is told
	he must log out, then his gateway
	will upgrade to the new type

  */


#ifndef _included_changegatewayevent_h
#define _included_changegatewayevent_h


#include "world/scheduler/uplinkevent.h"


class GatewayDef;


class ChangeGatewayEvent : public UplinkEvent
{

public:

	GatewayDef* newgatewaydef;

public:

	ChangeGatewayEvent ();
	~ChangeGatewayEvent () override;

	void SetNewGateway ( GatewayDef *value );

	void Run () override;
	void RunWarning () override;

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

