
/*

  LocalInterfaceScreen object

	A slightly more customized version of Interface Screen

  */

#ifndef _included_localinterface_screen
#define _included_localinterface_screen


#include "eclipse.h"
#include "interface/interfacescreen.h"


//! Screen that provides basic functionality of other screens
class LocalInterfaceScreen : public InterfaceScreen  
{

protected:

    /**
     * Renders the background graphics for window
     * @param button The background button
     * @param highlighted
     * @param clicked
     * @note highlighted and clicked are unused
     */
	static void BackgroundDraw ( Button *button, bool highlighted, bool clicked );

public:

	LocalInterfaceScreen();
	~LocalInterfaceScreen() override;

	void Create () override;

	/**
	 * Creates a screen panel with given height
	 * @param panelheight Height of the panel to create
	 */
	void CreateHeight ( int panelheight );
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

	/**
	 * Gets the screen object, asserts that requested screen ID matches current screen ID
	 * @param screenID The screen ID to get
	 * @return The LocalInterfaceScreen object
	 */
	static LocalInterfaceScreen *GetInterfaceScreen ( int screenID );

};

#endif
