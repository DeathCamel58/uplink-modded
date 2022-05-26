
/*

  Security Screen Interface

	Gives the player access to all installed
	security systems on the computer

  */


#ifndef included_securityscreeninterface_h
#define included_securityscreeninterface_h

// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

// ============================================================================


class SecurityScreenInterface : public RemoteInterfaceScreen
{

protected:

	static time_t lastupdate;

	static void SystemTitleDraw ( Button *button, bool highlighted, bool clicked );
	static void StatusClick ( Button *button );

	static void CloseClick ( Button *button );

public:

    bool EscapeKeyPressed () override;

	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};


#endif

