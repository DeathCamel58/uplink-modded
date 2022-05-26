// MissionInterface.h: interface for the MissionInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_missioninterface_h
#define included_missioninterface_h

// ============================================================================

#include "interface/localinterface/localinterfacescreen.h"

class Mission;

// ============================================================================


class MissionInterface : public LocalInterfaceScreen  
{

protected:

	int index;
	time_t timesync;
	Mission *mission;

protected:

	static void TitleClick ( Button *button );
	static void AbandonClick ( Button *button );
	static void ReplyClick ( Button *button );

public:

	MissionInterface ();
	~MissionInterface () override;

	void SetMission ( int index );

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
