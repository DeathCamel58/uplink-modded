
/*

  Generic Options Screen

	Picks out all options of a certain type
	and allows you to edit them

  */
  

#ifndef included_genericoptionsinterface_h
#define included_genericoptionsinterface_h

#include "eclipse.h"

#include "mainmenu/mainmenuscreen.h"




class GenericOptionsInterface : public MainMenuScreen
{

protected:

	char optionTYPE [16]{};

protected:

	static void ApplyClick ( Button *button );
	static void ReturnToMainMenuClick ( Button *button );

	static void ToggleBoxDraw ( Button *button, bool highlighted, bool clicked );
	static void ToggleBoxClick ( Button *button );

    static void ExitGameClick ( Button *button );

public:

	GenericOptionsInterface ();
	~GenericOptionsInterface () override;

	void SetOptionTYPE ( char *newtype );						// eg GAME, SOUND etc
	static void ChangeOptionValue ( char *option, int newvalue );

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};



#endif


