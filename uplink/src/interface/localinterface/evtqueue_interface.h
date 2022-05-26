
/*

  Event Queue Interface

	Debug screen for watching the Event queue

  */


#ifndef included_evtqueueinterface_h
#define included_evtqueueinterface_h

// ============================================================================

#include "eclipse.h"

#include "interface/localinterface/localinterfacescreen.h"

// ============================================================================


class EventQueueInterface : public LocalInterfaceScreen
{

protected:

	static int baseoffset;
	static int previousupdate;

protected:

	static void TitleClick ( Button *button );
	static void ScrollUpClick   ( Button *button );
	static void ScrollDownClick ( Button *button );

	static void EventDraw ( Button *button, bool highlighted, bool clicked );
	static void EventClick ( Button *button );

	static void DeleteEventDraw ( Button *button, bool highlighted, bool clicked );
	static void DeleteEventClick ( Button *button );
    
	static void FullDetailsCloseClick ( Button *button );

public:

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};


#endif

