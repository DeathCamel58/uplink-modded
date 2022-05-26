

/*

  Tutorial program

	Sits on screen and tells you what to do 
	every now and again


  */

#ifndef included_tutorial_h
#define included_tutorial_h

#include "eclipse.h"

#include "interface/taskmanager/uplinktask.h"


class Tutorial : public UplinkTask
{

protected:

	static void CloseClick ( Button *button );
	static void TitleClick ( Button *button );
	static void SkipClick ( Button *button );

	static void NextDraw ( Button *button, bool highlighted, bool clicked );
	static void NextMouseMove ( Button *button );
	static void NextMouseDown ( Button *button );
	static void NextClick ( Button *button );

	static void MainTextDraw ( Button *button, bool highlighted, bool clicked );

	static void MenuButtonClick ( Button *button );

protected:

	int current_tutorial;
	int current_part;
	
	bool nextvisible;									// True if the NEXT button should be visible
	bool nextclicked;									// True if the NEXT button has been clicked
	bool menu;											// True if we're in the selection menu

public:

	Tutorial ();
	~Tutorial () override;

	void RunMenu ();
	void RunTutorial ( int tutorial, int part );
    void RunTutorial1 ( int part );
    void RunTutorial2 ( int part );
    void RunTutorial3 ( int part );
	bool HasCompletedCurrentSection () const;
	

	void Initialise () override;
	void Tick ( int n ) override;

	void MoveTo ( int x, int y, int time_ms ) override;

	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;
	bool IsInterfaceVisible () override;

	void MoveMenuTo ( int x, int y, int time_ms );

	void CreateMenuInterface ();
	void RemoveMenuInterface ();
	void ShowMenuInterface ();
	bool IsMenuInterfaceVisible ();

};



#endif

