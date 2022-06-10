
#ifndef included_mainmenuscreen_h
#define included_mainmenuscreen_h

#include "tosser.h"

//! Main Menu Screen base class
/**
 * All screen accessable via the main menu should be derived from this
 */
class MainMenuScreen
{

protected:
    /**
     * A list of all buttons
     */
	DArray<char *> *interface_buttons;

    /**
     * Add a new button to the interface
     * @param x X coordinate of button
     * @param y Y coordinate of button
     * @param width Width of button
     * @param height Height of button
     * @param caption Caption of button
     * @param name Name of button
     */
	virtual void RegisterButton ( int x, int y, int width, int height,
								  char *caption, char *name );

    /**
     * Add a new button to the interface
     * @param x X coordinate of button
     * @param y Y coordinate of button
     * @param width Width of button
     * @param height Height of button
     * @param caption Caption of button
     * @param tooltip Tooltip of button
     * @param name Name of button
     */
	virtual void RegisterButton ( int x, int y, int width, int height,
								  char *caption, char *tooltip, char *name );
public:

	MainMenuScreen();
	virtual ~MainMenuScreen();

	/**
	 * Create the screen
	 */
	virtual void Create ();

	/**
	 * Destroy the screen
	 */
	virtual void Remove ();

	/**
	 * Update the screen
	 */
	virtual void Update ();

	/**
	 * Is the screen currently visible?
	 * @return true if screen is visible
	 */
	virtual bool IsVisible ();

	/**
	 * Get the ID of the screen
	 * @return ID of the screen
	 */
	virtual int  ScreenID ();			

	/**
	 * No clue
	 * @note Original code had comment "Return true to block further usage of this return". This is odd, because this *does not* return true.
	 * @return false under all conditions
	 */
	virtual bool ReturnKeyPressed ();

};

#endif
