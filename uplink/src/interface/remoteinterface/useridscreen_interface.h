

#ifndef included_useridscreeninterface_h
#define included_useridscreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class UserIDScreen;

// ============================================================================


class UserIDScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void UserIDButtonDraw ( Button *button, bool highlighted, bool clicked );

	static void CodeButtonDraw ( Button *button, bool highlighted, bool clicked );
	static void CodeButtonClick ( Button *button );

	static void AccessCodeClick ( Button *button );

	static void BypassClick ( Button *button );
	static void ProceedClick ( Button *button );

public:

	bool ReturnKeyPressed () override;

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	UserIDScreen *GetComputerScreen ();

};


#endif
