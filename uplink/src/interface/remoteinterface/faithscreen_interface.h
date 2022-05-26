
/*

  Faith Screen Interface

	Shows the progress of Faith V Revelation

  */


// ============================================================================

#ifndef included_faithscreeninterface_h
#define included_faithscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

// ============================================================================



class FaithScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void CloseClick ( Button *button );

public:

	FaithScreenInterface ();
	~FaithScreenInterface () override;

    bool EscapeKeyPressed () override;

	void Create () override;
	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};



#endif

