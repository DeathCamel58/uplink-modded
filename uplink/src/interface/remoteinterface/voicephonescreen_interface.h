
#ifndef included_voicephonescreeninterface_h
#define included_voicephonescreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

#define VOICESTATUS_NONE		0
#define VOICESTATUS_RINGING		1
#define VOICESTATUS_CONNECTED	2
#define VOICESTATUS_TALKING		3
#define VOICESTATUS_HUNGUP		5

//! RemoteInterfaceScreen for phone screen
/**
 * Pops up when the user dials somebody's phone number. Basically made up of ringing sounds followed by voice samples.
 */
class VoicePhoneScreenInterface : public RemoteInterfaceScreen
{

protected:

    /**
     * Appends given text to the string shown
     * @param newtext The string to append
     */
	static void AddToText (const string& newtext );

protected:

    /**
     * The voice sound file number to use
     */
    int voiceindex;

    /**
     * How many rings should occur before being answered
     */
	int numrings;

	/**
	 * The number of words the receiver says
	 */
	int numwords;
	int timesync;

	/**
	 * Was the phone be answered
	 */
	bool answer;

public:

    /**
     * The current status of the phone call
     * @note This can be VOICESTATUS_NONE, VOICESTATUS_RINGING, VOICESTATUS_CONNECTED, VOICESTATUS_TALKING, or VOICESTATUS_HUNGUP
     */
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
