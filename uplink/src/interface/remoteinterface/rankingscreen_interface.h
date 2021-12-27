
/*

  Rankings Interface

	Appears on the Uplink internal services machine
	shows agents ranked in score order

  */


#ifndef _included_rankingsinterface_h
#define _included_rankingsinterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;



class RankingScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void ExitClick ( Button *button );

	static void ActiveAgentDraw ( Button *button, bool highlighted, bool clicked );
	static void NonActiveAgentDraw ( Button *button, bool highlighted, bool clicked );

	static void PlayerDraw ( Button *button, bool highlighted, bool clicked );

public:

	bool ReturnKeyPressed () override;
    bool EscapeKeyPressed () override;

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};


#endif

