
#ifndef included_hardwareinterface_h
#define included_hardwareinterface_h


#include "interface/localinterface/localinterfacescreen.h"


//! Screen that shows current hardware
class HWInterface : public LocalInterfaceScreen
{

protected:

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
	static void TitleClick ( Button *button );

	/**
	 * Opens the gateway screen
	 * @param button Button to open screen
	 */
	static void ShowGatewayClick ( Button *button );

	/**
	 * Renders the section titles
	 * @param button The button that contains the title information
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void MiniTitleDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Creates the screen
	 */
	static void CreateHWInterface ();

	/**
	 * Removes the screen
	 */
	static void RemoveHWInterface ();

	/**
	 * Is the screen visible?
	 * @return true if it is
	 */
	static bool IsVisibleHWInterface ();

	static void BackClick ( Button *button );
	
public:

	void Create () override;
	void Update () override;
	void Remove () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif

