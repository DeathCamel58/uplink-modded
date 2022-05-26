
/*

  RemoteInterfaceScreen class object
	
	A slightly more customized version of InterfaceScreen
	which has a link to a ComputerScreen object

  */

#ifndef included_remoteinterfacescreen_h
#define included_remoteinterfacescreen_h

// ============================================================================

#include "eclipse.h"

#include "interface/interfacescreen.h"

class ComputerScreen;

// ============================================================================


class RemoteInterfaceScreen : public InterfaceScreen  
{

protected:

	static void DrawMainTitle ( Button *button, bool highlighted, bool clicked );
	static void DrawSubTitle ( Button *button, bool highlighted, bool clicked );

protected:

	ComputerScreen *cs;

public:

	RemoteInterfaceScreen();
	~RemoteInterfaceScreen() override;
	
	void Create () override;											// Provided
	virtual void Create ( ComputerScreen *newcs );

	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

	ComputerScreen *GetComputerScreen ();			// Should override this with correct data type

	virtual bool ReturnKeyPressed ();				// Return true to block further usage of this return
    virtual bool EscapeKeyPressed ();               // Return true to block further usage of this escape

	static RemoteInterfaceScreen *GetInterfaceScreen ( int screenID );					// Returns the This object, asserting it is the correct type

};

#endif
