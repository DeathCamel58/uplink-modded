
/*

  Connection Lost Interface

	Appears after your connection from your home PC to your Gateway is lost
	after you nuke it usually

  */


#ifndef included_connectionlostinterface_h
#define included_connectionlostinterface_h

#include "mainmenu/mainmenuscreen.h"


class ConnectionLostInterface : public MainMenuScreen
{

protected:

	time_t starttime;

protected:

	static void ReturnToMainMenuClick ( Button *button );

public:

	ConnectionLostInterface ();
	~ConnectionLostInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};




#endif

