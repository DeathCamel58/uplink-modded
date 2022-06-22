// UplinkTask.h: interface for the UplinkTask class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_uplinktask_h
#define included_uplinktask_h

// ============================================================================

#include "task.h"
#include "app/uplinkobject.h"

// ============================================================================


class UplinkTask : public Task  
{

protected:

    /**
     * The object to target
     */
	UplinkObject *targetobject;
	string targetstring;
	int			  targetint;
	
public:

	float version;
	bool followmouse;

public:

	UplinkTask();
	~UplinkTask() override;

	void SetVersion ( float newversion );
	void SetFollowMouse ( bool newfollowmouse );

    /**
     * Sets the target of the program
     * @param newtargetobject The object to target
     * @param newtargetstring
     * @param targetint
     */
	virtual void SetTarget (UplinkObject *newtargetobject, const string &newtargetstring, int targetint );

	/**
	 * Moves the program to given coordinates over a set period of time
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param time_ms The time in milliseconds the movement should take
	 */
	virtual void MoveTo ( int x, int y, int time_ms );
	
};

#endif 
