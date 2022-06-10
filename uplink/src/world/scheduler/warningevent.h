
#ifndef included_warningevent_h
#define included_warningevent_h


#include "world/scheduler/uplinkevent.h"


//! Warning event
/**
 * Warning event
 * Triggered as a warning that another Uplink Event is about to happen
 * e.g. Your gateway will be seized in 10 minutes
 */
class WarningEvent : public UplinkEvent
{

protected:

    /**
     * The event that is going to occur
     */
	UplinkEvent *event;

public:

	WarningEvent ();
	~WarningEvent () override;

	/**
	 * Sets the event to warn about
	 * @param newevent Event to warn about
	 */
	void SetEvent ( UplinkEvent *newevent );

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

