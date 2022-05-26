
/*

  Definition of a Dialog Screen Interface

  */

#ifndef included_dialogscreeninterface_h
#define included_dialogscreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class DialogScreen;
class DialogScreenWidget;

// ============================================================================


class DialogScreenInterface : public RemoteInterfaceScreen  
{

protected:

	static void PasswordBoxDraw		( Button *button, bool highlighted, bool clicked );

	static void NextPageClick		( Button *button );
	static void ScriptButtonClick	( Button *button );
	
public:

	DialogScreenInterface();
	~DialogScreenInterface() override;

	bool ReturnKeyPressed () override;
    bool EscapeKeyPressed () override;

	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	bool IsVisible () override;

	int  ScreenID () override;

	DialogScreen *GetComputerScreen ();				// Should override this with correct data type

	static void RemoveWidget ( DialogScreenWidget *dsw, Computer *comp );

};

#endif
