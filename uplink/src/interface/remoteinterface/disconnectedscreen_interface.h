

#ifndef _included_disconnectedscreeninterface_h
#define _included_disconnectedscreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class DisconnectedScreen;

// ============================================================================


class DisconnectedScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void Click ( Button *button );

public:

	bool ReturnKeyPressed () override;

	void Create () override;
	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	bool IsVisible () override;

	int ScreenID () override;

	DisconnectedScreen *GetComputerScreen ();

};


#endif
