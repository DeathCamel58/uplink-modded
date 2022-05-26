

#ifndef included_messagescreeninterface_h
#define included_messagescreeninterface_h


// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class MessageScreen;

// ============================================================================


class MessageScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void Click ( Button *button );
	static void MailMeClick ( Button *button );

public:

	bool ReturnKeyPressed () override;

	void Create () override;
	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	bool IsVisible () override;

	int ScreenID () override;

	MessageScreen *GetComputerScreen ();

};


#endif
