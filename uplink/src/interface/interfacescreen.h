
/*

  InterfaceScreen class object

	Part of the Interface sub-system
	Base class for defining a screen that will be in the interface
	 for example the InRealLife screen, or a password screen etc

  */


#ifndef included_interfacescreen_h
#define included_interfacescreen_h

#include "app/uplinkobject.h"

#include "tosser.h"

class InterfaceScreen : public UplinkObject
{

protected:
	DArray<char *> *interface_buttons;
	virtual void RegisterButton ( int x, int y, int width, int height,
								  char *caption, char *name );
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
