
/*

  Voice Phone System

	Pops up when the user dials somebody's
	phone number.   Basically made up of
	ringing sounds followed by voice samples.

  */

#ifndef _included_voicephonescreeninterface_h
#define _included_voicephonescreeninterface_h

// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

#define VOICESTATUS_NONE		0
#define VOICESTATUS_RINGING		1
#define VOICESTATUS_CONNECTED	2
#define VOICESTATUS_TALKING		3
#define VOICESTATUS_HUNGUP		5

// ============================================================================



class VoicePhoneScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void AddToText (const string& newtext );

protected:

    int voiceindex;
	int numrings;
	int numwords;
	int timesync;
	bool answer;

public:

	int STATUS;

public:

	VoicePhoneScreenInterface ();
	~VoicePhoneScreenInterface () override;

	bool ReturnKeyPressed () override;

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};


#endif
