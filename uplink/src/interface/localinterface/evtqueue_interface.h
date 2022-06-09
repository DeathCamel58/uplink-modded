
#ifndef included_evtqueueinterface_h
#define included_evtqueueinterface_h

// ============================================================================

#include "eclipse.h"

#include "interface/localinterface/localinterfacescreen.h"

// ============================================================================


//! Screen that shows scheduled events
/**
 * @note This is a debug screen for watching the event queue
 */
class EventQueueInterface : public LocalInterfaceScreen
{

protected:

	static int baseoffset;

	/**
	 * The time the list was last updated
	 */
	static int previousupdate;

protected:

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
	static void TitleClick ( Button *button );

	/**
	 * Scrolls the scrollbox up
	 * @param button
     * @note button is unused
	 */
	static void ScrollUpClick   ( Button *button );

    /**
     * Scrolls the scrollbox down
     * @param button
     * @note button is unused
     */
	static void ScrollDownClick ( Button *button );

	/**
	 * Renders each event in the list
	 * @param button The button to render the event to
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void EventDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders the event full details popup window
	 * @param button The button clicked on to open event
	 */
	static void EventClick ( Button *button );

	/**
	 * Renders the delete button
	 * @param button The delete button
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void DeleteEventDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Deletes an event from the schedule
	 * @param button The button clicked to delete the event
	 */
	static void DeleteEventClick ( Button *button );

	/**
	 * Removes the full details window
	 * @param button
	 */
	static void FullDetailsCloseClick ( Button *button );
    static void ScrollChange (const string &scrollname, int newValue );

    /**
     * Calculates how many events can be listed for given height
     * @param height height of the area
     * @return The number of events listable in space
     */
    static int NumItemsOnScreen(int height);

public:

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};


#endif

