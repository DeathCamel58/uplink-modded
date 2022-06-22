
#ifndef included_swinterface_h
#define included_swinterface_h

#include "eclipse.h"

#include "interface/localinterface/localinterfacescreen.h"

//! LocalInterfaceScreen that displays "Start" button
/**
 * This also handles all menus that come off of the "Start" button
 */
class SWInterface : public LocalInterfaceScreen 
{

protected:

    /**
     * The currently open sub menu
     */
	static int currentsubmenu;

	/**
	 *
	 */
	static int currentprogrambutton;					// Used to bring up Version list after

    /**
     * The next time to update the menu at
     */
	static int timesync;

protected:

    /**
     * Runs currently selected software
     * @param button Button for current software
     */
	static void SoftwareClick		( Button *button );

	/**
	 * Render the button in the software list
	 * @param button
	 * @param highlighted
	 * @param clicked
	 */
	static void SoftwareDraw		( Button *button, bool highlighted, bool clicked );

	/**
	 * 	Don't do anything immediately, but set some timer events up so that a list of all versions of this software item appear in a new sub menu in a few milliseconds
	 * @param button Button to highlight
	 */
	static void SoftwareHighlight   ( Button *button );

	/**
	 * Look through the player's databank to see if this type of program shows up
	 * @param softwareTYPE Software type to look for
	 * @return true if user has it
	 */
	static bool HasSoftwareType     ( int softwareTYPE );

	/**
	 * Toggles a software type submenu open and closed, and renders all elements in the submenu
	 * @param softwareTYPE Software type for submenu
	 * @param x X coordinate of bottom left of submenu
	 * @param y Y coordinate of bottom left of submenu
	 */
	static void ToggleSubMenu       ( int softwareTYPE, int x, int y );

	/**
	 * Is the submenu visible
	 * @param softwareTYPE Software submenu to check
	 * @return true if visible
	 */
	static bool IsVisibleSubMenu	( int softwareTYPE );

	/**
	 * Does the player have more than one version of a software?
	 * @param program Program to check
	 * @return true if they have more than one version
	 */
	static bool HasVersionMenu		 (const string &program );

	/**
	 * Toggles a submenu open and closed for a version list, and renders all elements in the submenu
	 * @param program Program to open submenu for
	 * @param x X coordinate of submenu
	 * @param y Y coordinate of submenu
	 */
	static void ToggleVersionMenu    (const string &program, int x, int y );

	/**
	 * Is the version menu open?
	 * @param program Program submenu to check
	 * @return true if submenu is open
	 */
	static bool IsVisibleVersionMenu (const string &program );

	/**
	 * Renders an item in the start menu
	 * @param button Button to render
	 * @param highlighted If the button should be highlighted
	 * @param clicked TODO: Document
	 */
	static void StartMenuItemDraw				  ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders the submenu of an item if it doesn't exist
	 * @param button Button hovered over
	 */
	static void StartMenuItemHighlight			  ( Button *button );

	/**
	 * Renders a software category item with no submenu
	 * @param button Button to render
	 * @param highlighted TODO: Document
	 * @param clickedTODO: Document
	 */
	static void StartMenuItemDrawUnavailable	  ( Button *button, bool highlighted, bool clicked );

	/**
	 * Highlight a software category item with no submenu
	 * @param button Button to render
	 */
	static void StartMenuItemHighlightUnavailable ( Button *button );
		
public:

    /**
     * Toggles the main software menu
     * @note This only renders the main menu, not the submenus
     */
	static void ToggleSoftwareMenu  ();

	/**
	 * Is the main software menu visible?
	 * @return true if it is
	 */
	static bool IsVisibleSoftwareMenu ();

	// Common functions

	void Create () override;
	void Remove () override;
	void Update () override;
	
	bool IsVisible () override;

};


#endif

