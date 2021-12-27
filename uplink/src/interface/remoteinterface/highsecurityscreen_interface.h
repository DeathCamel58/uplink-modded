
/*

  High Security Screen Interface

	Renders a screen which looks like a menu
	showing all the security logins which 
	must be bypassed before access can be granted.

  */

#ifndef _included_highsecurityscreeninterface_h
#define _included_highsecurityscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class HighSecurityScreen;
class ComputerScreen;



class HighSecurityScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void SystemTitleDraw ( Button *button, bool highlighted, bool clicked );
	static void SystemClick ( Button *button );

	static void BypassClick ( Button *button );
	static void ProceedClick ( Button *button );

public:
	
	HighSecurityScreenInterface ();
	~HighSecurityScreenInterface () override;

	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	bool ReturnKeyPressed () override;

	HighSecurityScreen *GetComputerScreen ();

};



#endif

