
#ifndef included_analyserinterface_h
#define included_analyserinterface_h


#include "world/vlocation.h"

#include "interface/localinterface/localinterfacescreen.h"

//! LocalInterfaceScreen that shows remote host security systems
class AnalyserInterface : public LocalInterfaceScreen
{

protected:

    /**
     * The IP of the remote host
     */
	static char remotehost [SIZE_VLOCATION_IP];

	/**
	 * Moving dots pattern
	 */
	static int moving_stipplepattern;

	/**
	 * Stationary dots pattern
	 */
	static int static_stipplepattern;

    /**
     * The time the security systems last updated
     */
	time_t lastupdate;

	/**
	 * Time to cycle the stipplepattern at
	 */
	int stippleupdate;

protected:

    /**
     * Cycles the stipplepattern
     */
	static void CycleStipplePattern ();

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
	static void TitleClick ( Button *button );

	/**
	 * Detects what security system was clicked on
	 * @param button The button clicked on
	 */
	static void SystemClick ( Button *button );

	/**
	 * Renders the screen
	 * @param button Button with background image
	 * @param highlighted
	 * @param clicked
     * @note highlighted and clicked are unused
	 */
	static void ConnectionDraw ( Button *button, bool highlighted, bool clicked );

public:

	AnalyserInterface ();
	~AnalyserInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};


#endif

