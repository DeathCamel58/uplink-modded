
//	Record Screen Interface

#ifndef _included_recordscreeninterface_h
#define _included_recordscreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

// ============================================================================



class RecordScreenInterface : public RemoteInterfaceScreen
{

protected:

	static int recordindex;

protected:

	static void ScrollLeftClick ( Button *button );
	static void ScrollRightClick ( Button *button );

	static void CommitClick ( Button *button );
	static void CloseClick ( Button *button );

	static void CreateRecord ( int index );

public:

	void Create ( ComputerScreen *newcs ) override;
		
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

	GenericScreen *GetComputerScreen ();		

};


#endif

