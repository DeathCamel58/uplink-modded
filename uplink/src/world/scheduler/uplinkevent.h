
#ifndef included_uplinkevent_h
#define included_uplinkevent_h

// ============================================================================

#include "app/uplinkobject.h"

#include "world/date.h"

// ============================================================================


//! Uplink Event object
/**
 * Base class for all future events that can occur
 */
class UplinkEvent : public UplinkObject
{

public:

    /**
     * Time to run at
     */
	Date rundate;

public:

	UplinkEvent ();
	~UplinkEvent () override;

	/**
	 * Sets the time to run at
	 * @param newrundate Time to run at
	 */
	void SetRunDate ( Date *newrundate );

	/**
	 * Runs the event
	 */
	virtual void Run ();

	/**
	 * Warns user that the event is about to occur
	 * @note If not overridden, this does nothing
	 */
	virtual void RunWarning ();


	/**
	 * Get the short string version of the event
	 * @return Short string
	 */
	virtual string GetShortString ();

	/**
	 * Get the long string version of the event
	 * @return
	 */
	virtual string GetLongString ();

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif

