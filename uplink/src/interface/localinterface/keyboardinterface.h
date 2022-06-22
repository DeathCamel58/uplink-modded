
#ifndef included_keyboard_h
#define included_keyboard_h


#include "interface/localinterface/localinterfacescreen.h"

//! LocalInterfaceScreen for the Virtual Keyboard Tool
/**
 * Brings up a keyboard on screen
 * The player can use it to enter text
 */
class KeyboardInterface : public LocalInterfaceScreen  
{

protected:

    /**
     * The button to input text into
     */
	static char *targetbuttonname;

protected:

    /**
     * Destroys the keyboard
     * @param button
     * @note button is unused
     */
	static void RemoveKeyboardInterface ( Button *button );

	/**
	 * Puts character into target button caption
	 * @param button Character button that was clicked
	 */
	static void Keypress ( Button *button );
	
public:

	KeyboardInterface();
	~KeyboardInterface() override;

	/**
	 * Sets the targetbuttonname
	 * @param bname Value to set targetbuttonname to
	 */
	static void SetTargetButton ( char *bname );

	/**
	 * Creates the keyboard
	 * @param x X position of keyboard
	 * @param y Y position of keyboard
	 */
	void Create ( int x, int y );
	void Remove () override;
	bool IsVisible () override;

};

#endif
