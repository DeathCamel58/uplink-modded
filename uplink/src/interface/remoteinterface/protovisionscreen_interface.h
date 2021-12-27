

#ifndef _included_protovisionscreeninterface_h
#define _included_protovisionsscreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class GenericScreen;

// ============================================================================


class ProtovisionScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void Close ( Button *button );
    
    static void NuclearWarDraw ( Button *button, bool highlighted, bool clicked );
    static void NuclearWar ( Button *button );
    
public:

	bool ReturnKeyPressed () override;

	void Create () override;
	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
    void Update () override;
    bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};


#endif
