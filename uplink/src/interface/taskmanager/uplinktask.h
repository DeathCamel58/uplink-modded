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

	virtual void SetTarget (UplinkObject *newtargetobject, const string &newtargetstring, int targetint );
	virtual void MoveTo ( int x, int y, int time_ms );	
	
};

#endif 
