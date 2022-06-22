
#ifndef included_logininterface_h
#define included_logininterface_h

#include "eclipse.h"

#include "mainmenu/mainmenuscreen.h"

//! Login interface MainMenuScreen
/**
 * Essentially the first main menu - Load New Exit etc
 */
class LoginInterface : public MainMenuScreen
{

protected:

	static void CreateExistingGames ();
	static void RemoveExistingGames ();

	static void NewGameClick     ( Button *button );
	static void RetireAgentClick ( Button *button );
	static void RetireAgentMsgboxClick ( Button *button );
	static void ProceedClick     ( Button *button );
	static void OptionsClick     ( Button *button );
	static void ExitGameClick    ( Button *button );
	
	static void CommsClick	     ( Button *button );
	static void StatusClick	     ( Button *button );

	static void LargeTextBoxDraw ( Button *button, bool highlighted, bool clicked );
	static void UserIDButtonDraw ( Button *button, bool highlighted, bool clicked );
	static void CodeButtonDraw   ( Button *button, bool highlighted, bool clicked );

	static void UserNameClick    ( Button *button );

public:

	LoginInterface ();
	~LoginInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

	bool ReturnKeyPressed () override;

};

#endif
