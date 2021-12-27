
/*

  EmailInterface definition

	Views an email (Message) object on screen

  */

#ifndef _included_emailinterface_h
#define _included_emailinterface_h

#include "eclipse.h"

// ============================================================================

#include "interface/localinterface/localinterfacescreen.h"

class Message;

// ============================================================================


class EmailInterface : public LocalInterfaceScreen  
{

protected:

	int index;
	time_t timesync;
	Message *message;

protected:

	static void TitleClick ( Button *button );
		
	static void EmailClose ( Button *button );
    static void EmailReply ( Button *button );
	static void EmailDelete ( Button *button );	

public:

	EmailInterface();
	~EmailInterface() override;

	void SetMessage ( int index );

	void Create () override;

	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};

#endif
