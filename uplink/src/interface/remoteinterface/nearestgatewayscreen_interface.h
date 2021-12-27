
/*

  Nearest Gateway screen interface

	Allows the player to select his nearest gateway
	from a world map with the major cities listed on it

  */


#ifndef _included_nearestgatewayscreeninterface_h
#define _included_nearestgatewayscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;


class NearestGatewayScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void DrawBlack ( Button *button, bool highlighted, bool clicked );

	static void DrawLocation ( Button *button, bool highlighted, bool clicked );
	static void ClickLocation ( Button *button );

    static void DrawMainMap ( Button *button, bool highlighted, bool clicked );

public:

	NearestGatewayScreenInterface ();
	~NearestGatewayScreenInterface () override;

	bool ReturnKeyPressed () override;

	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};


#endif

