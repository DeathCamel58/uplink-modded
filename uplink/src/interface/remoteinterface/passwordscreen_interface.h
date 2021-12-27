

#ifndef _included_passwordscreeninterface_h
#define _included_passwordscreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class PasswordScreen;

// ============================================================================


class PasswordScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void CodeButtonDraw ( Button *button, bool highlighted, bool clicked );

	static void CursorFlash ();
	static void PasswordClick ( Button *button );

	static void AccessCodeClick ( Button *button );

	static void BypassClick ( Button *button );

public:

	void NextPage ();

	bool ReturnKeyPressed () override;

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	PasswordScreen *GetComputerScreen ();

};


#endif
