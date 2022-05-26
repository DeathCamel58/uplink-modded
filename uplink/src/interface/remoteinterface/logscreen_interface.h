

#ifndef included_logscreeninterface_h
#define included_logscreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class LogScreen;

// ============================================================================


class LogScreenInterface : public RemoteInterfaceScreen
{

protected:

	static int baseoffset;
	static int previousnumlogs;

protected:

	static void LogDraw			( Button *button, bool highlighted, bool clicked );

/*
	static void ScrollUpClick	( Button *button );
	static void ScrollDownClick ( Button *button );
*/

	static int GetNbItems ( DArray<class AccessLog *> *logs );
	static int GetFirstItem ( DArray<class AccessLog *> *logs );
	static int GetLastItem ( DArray<class AccessLog *> *logs );

    static void CreateScrollBar ( int nbItems );
    static void ScrollChange (const string &scrollname, int newValue );

	static void LogClick		( Button *button );
	static void CloseClick		( Button *button );

public:

    bool EscapeKeyPressed () override;

	void Create () override;
	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	LogScreen *GetComputerScreen ();

};


#endif
