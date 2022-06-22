
#ifndef included_interfacescreen_h
#define included_interfacescreen_h

#include "app/uplinkobject.h"

#include "tosser.h"

//! InterfaceScreen Class UplinkObject
/**
 * Part of the interface sub-system
 * Base class for defining a screen that will be in the interface.
 * For example, the InRealLife screen, a password screen, etc.
 */
class InterfaceScreen : public UplinkObject
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

	InterfaceScreen();
	~InterfaceScreen() override;

	/**
	 * Creates the screen
	 */
	virtual void Create ();

	/**
	 * Removes the screen
	 */
	virtual void Remove ();

	/**
	 * Updates the screen.
	 * This can update animations, or check if the data on the screen changed.
	 */
	void Update () override;

	/**
	 * Is the screen visible?
	 * @return true if screen is visible
	 */
	virtual bool IsVisible ();

	/**
	 * Gets the current screen ID
	 * @return The screen ID
	 */
	virtual int  ScreenID ();			

};


#endif 
