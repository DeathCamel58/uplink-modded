
/*

	Definition of News screen interface

  */

#ifndef included_newsscreeninterface_h
#define included_newsscreeninterface_h

// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class GenericScreen;

// ============================================================================


class NewsScreenInterface : public RemoteInterfaceScreen
{

protected:

	static int baseoffset;
	static int currentselect;
	static int previousnummessages;

protected:

	static void DrawNewsButton      ( Button *button, bool highlighted, bool clicked );
	static void ClickNewsButton     ( Button *button );
	static void MousedownNewsButton ( Button *button );
	static void HighlightNewsButton ( Button *button );
	
	static void DrawDetails     ( Button *button, bool highlighted, bool clicked );

	static void ExitClick		( Button *button );
    static void ScrollChange    (const string &scrollname, int newValue );

public:

    bool EscapeKeyPressed () override;

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();				// Should override this with correct data type

};

#endif

