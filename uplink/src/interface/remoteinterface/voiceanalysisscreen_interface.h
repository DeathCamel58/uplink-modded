
/*

  Voice analysis screen

	Asks the user to read out a phrase
	into his microphone
	Analyses the pattern and lets you
	in only if correct

  */



#ifndef included_voiceanalysisinterface_h
#define included_voiceanalysisinterface_h

// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

#define VOICE_NUMSAMPLES 25

#define VOICEANALYSISSCREEN_NONE		0
#define VOICEANALYSISSCREEN_LISTENING	1
#define VOICEANALYSISSCREEN_ANALYSING	2
#define VOICEANALYSISSCREEN_PASSED		3
#define VOICEANALYSISSCREEN_FAILED		4

// ============================================================================


class VoiceAnalysisScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void DrawBackground ( Button *button, bool highlighted, bool clicked );
	static void DrawAnalysis ( Button *button, bool highlighted, bool clicked );

	static void ClickAccess ( Button *button );

protected:

	int sample[VOICE_NUMSAMPLES];
	int lastupdate;
	int STATUS;

public:

	VoiceAnalysisScreenInterface ();
	~VoiceAnalysisScreenInterface () override;

	void StartVoicePlayback ();
	void FinishVoicePlayback (const string& voicename );

	bool ReturnKeyPressed () override;

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};


#endif

