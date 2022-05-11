
/*

  SW interface

	Handles the "Start" button and all the menus that come off it
	
	*/



#ifndef _included_swinterface_h
#define _included_swinterface_h

#include "eclipse.h"

#include "interface/localinterface/localinterfacescreen.h"


class SWInterface : public LocalInterfaceScreen 
{

protected:

	static int currentsubmenu;
		
	static int currentprogrambutton;					// Used to bring up Version list after
	static int timesync;								// a few milliseconds

protected:

	static void SoftwareClick		( Button *button );
	static void SoftwareDraw		( Button *button, bool highlighted, bool clicked );
	static void SoftwareHighlight   ( Button *button );

	static bool HasSoftwareType     ( int softwareTYPE );
	static void ToggleSubMenu       ( int softwareTYPE, int x, int y );						// Co-ords of bottom-left
	static bool IsVisibleSubMenu	( int softwareTYPE );

	static bool HasVersionMenu		 (const string &program );										// If they have > 1 version
	static void ToggleVersionMenu    (const string &program, int x, int y );
	static bool IsVisibleVersionMenu (const string &program );

	static void StartMenuItemDraw				  ( Button *button, bool highlighted, bool clicked );	
	static void StartMenuItemHighlight			  ( Button *button );
	static void StartMenuItemDrawUnavailable	  ( Button *button, bool highlighted, bool clicked );
	static void StartMenuItemHighlightUnavailable ( Button *button );
		
public:

	static void ToggleSoftwareMenu  ();
	static bool IsVisibleSoftwareMenu ();

	// Common functions

	void Create () override;
	void Remove () override;
	void Update () override;
	
	bool IsVisible () override;

};


#endif

