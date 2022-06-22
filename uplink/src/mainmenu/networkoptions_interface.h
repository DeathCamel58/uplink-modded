
#ifndef included_networkoptionsinterface_h
#define included_networkoptionsinterface_h

#include "eclipse.h"

#include "mainmenu/mainmenuscreen.h"

//! Network options interface MainMenuScreen
class NetworkOptionsInterface : public MainMenuScreen
{

protected:

	static int oldNetworkStatus;

    /**
     * Closes the program
     * @param button
     * @note button is unused
     */
	static void CloseClick ( Button *button );

	static void ServerButtonClick ( Button *button );
	static void ClientButtonClick ( Button *button );

	static void StatusButtonDraw ( Button *button, bool highlighted, bool clicked );
	static void ServerButtonDraw ( Button *button, bool highlighted, bool clicked );
	static void ClientButtonDraw ( Button *button, bool highlighted, bool clicked );

public:

	NetworkOptionsInterface ();
	~NetworkOptionsInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

	/**
	 * No clue
	 * @return true under all conditions
	 */
	bool ReturnKeyPressed () override;

};

#endif
