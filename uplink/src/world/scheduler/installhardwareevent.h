
/*

  Install Hardware Event

	Occurs when the player has ordered a hardware upgrade 
	for his gateway and it finally arrives.

  */


#ifndef included_installhardwareevent_h
#define included_installhardwareevent_h

// ============================================================================

#include "world/scheduler/uplinkevent.h"

class Sale;

// ============================================================================


class InstallHardwareEvent : public UplinkEvent
{

protected:

	Sale *hwsale;
	int version;

public:

	InstallHardwareEvent ();
	~InstallHardwareEvent () override;

	void SetHWSale ( Sale *newsale, int newversion );
	
	void Run () override;
	void RunWarning () override;

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
