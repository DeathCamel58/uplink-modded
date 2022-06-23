

#ifndef included_disconnectedscreeninterface_h
#define included_disconnectedscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class DisconnectedScreen;

//! RemoteInterfaceScreen for "Connection terminated by Gateway" screen
class DisconnectedScreenInterface : public RemoteInterfaceScreen
{

protected:

    /**
     * Handles click of "OK" Button
     * @param button OK Button
     */
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
