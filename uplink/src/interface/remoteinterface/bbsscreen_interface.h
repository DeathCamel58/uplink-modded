
#ifndef included_bbsscreeninterface_h
#define included_bbsscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class BBSScreen;

//! RemoteInterfaceScreen for the uplink internal services' "Mission list" screen
class BBSScreenInterface : public RemoteInterfaceScreen
{

protected:

    /**
     * The index of the first item to display in the scroll box
     */
	static int baseoffset;

	/**
	 * The index of the current Mission selected
	 */
	static int currentselect;

	static int previousnummessages;

	/**
	 * The last time the screen was re-rendered
	 */
	static int previousupdate;

protected:

    /**
     * Renders the button content (uses button's name to get the index for details)
     * @param button Button to render
     * @param highlighted Callback for highlight
     * @param clicked
     * @note clicked is unused
     */
	static void DrawBBSButton      ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders details and button highlight effect on click
	 * @param button Button that was clicked
	 */
	static void ClickBBSButton     ( Button *button );

	/**
	 * If a mission exists on the button clicked, process the click
	 * @param button Button that was clicked
	 */
	static void MousedownBBSButton ( Button *button );

    /**
     * If a mission exists on the button highlighted, process the click
     * @param button Button that was highlighted
     */
	static void HighlightBBSButton ( Button *button );

	/**
	 * Renders the details to screen
	 * @param button Button to render
	 * @param highlighted Callback on highlight
	 * @param clicked Callback on click
	 */
	static void DrawDetails     ( Button *button, bool highlighted, bool clicked );

	/**
	 * Accepts the currently selected Mission and gives it to the Player
	 * @param button
	 * @note button is unused
	 */
	static void AcceptClick		( Button *button );

	/**
	 * Passes the selected Mission to ContactScreenInterface and starts the contact
	 * @param button
	 * @note button is unused
	 */
	static void ContactClick	( Button *button );

	/**
	 * Returns to previous screen
	 * @param button
	 */
	static void ExitClick		( Button *button );

	/**
	 * Changes the offset for the scroll box items
	 * @param scrollname
	 * @param newValue The new offset
	 * @note scrollname is unused
	 */
	static void ScrollChange    (const string &scrollname, int newValue );

	/**
	 * Gets the number of items that can fit on the screen
	 * @return Number of items that will fit
	 */
    static int NumItemsOnScreen ();

public:

    bool EscapeKeyPressed () override;

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	BBSScreen *GetComputerScreen ();				// Should override this with correct data type

};

#endif

