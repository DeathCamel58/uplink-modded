
/*

  Warning event

	Triggered as a warning that another Uplink Event is about to happen
	eg Your gateway will be seized in 10 minutes

  */


#ifndef included_warningevent_h
#define included_warningevent_h


#include "world/scheduler/uplinkevent.h"


class WarningEvent : public UplinkEvent
{

protected:

	UplinkEvent *event;

public:

	WarningEvent ();
	~WarningEvent () override;

	void SetEvent ( UplinkEvent *newevent );

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

