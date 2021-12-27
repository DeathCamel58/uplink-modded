
/*

  LocalInterfaceScreen object

	A slightly more customized version of Interface Screen

  */

#ifndef _included_localinterface_screen
#define _included_localinterface_screen


#include "eclipse.h"
#include "interface/interfacescreen.h"


class LocalInterfaceScreen : public InterfaceScreen  
{

protected:

	static void BackgroundDraw ( Button *button, bool highlighted, bool clicked );

public:

	LocalInterfaceScreen();
	~LocalInterfaceScreen() override;

	void Create () override;
	void CreateHeight ( int panelheight );
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

	static LocalInterfaceScreen *GetInterfaceScreen ( int screenID );						// Returns this object, asserts screenID

};

#endif
