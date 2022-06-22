
#ifndef included_remoteinterfacescreen_h
#define included_remoteinterfacescreen_h

// ============================================================================

#include "eclipse.h"

#include "interface/interfacescreen.h"

class ComputerScreen;

// ============================================================================

//! RemoteInterfaceScreen class object
/**
 * A slightly more customized version of InterfaceScreen which has a link to a ComputerScreen object
 */
class RemoteInterfaceScreen : public InterfaceScreen  
{

protected:

    /**
     * Renders the main title of this object
     * @param button Button to render
     * @param highlighted
     * @param clicked
     * @note highlighted and clicked are unused
     */
	static void DrawMainTitle ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders the subtitle of this object
	 * @param button Button to render
	 * @param highlighted
	 * @param clicked
     * @note highlighted and clicked are unused
	 */
	static void DrawSubTitle ( Button *button, bool highlighted, bool clicked );

protected:

    /**
     * The ComputerScreen of this object
     */
	ComputerScreen *cs;

public:

	RemoteInterfaceScreen();
	~RemoteInterfaceScreen() override;

	void Create () override;

	/**
	 * Creates a ComputerScreen in this object
	 * @param newcs The ComputerScreen
	 */
	virtual void Create ( ComputerScreen *newcs );

	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

	/**
	 * Should override this with correct data type
	 * @return
	 */
	ComputerScreen *GetComputerScreen ();

	/**
	 * Return true to block further usage of this return
	 * @return
	 */
	virtual bool ReturnKeyPressed ();

	/**
	 * Return true to block further usage of this escape
	 * @return
	 */
    virtual bool EscapeKeyPressed ();

    /**
     * Returns this object, asserting it is the correct type
     * @param screenID screenID to assert the matches
     * @return The RemoteInterfaceScreen
     */
	static RemoteInterfaceScreen *GetInterfaceScreen ( int screenID );

};

#endif
