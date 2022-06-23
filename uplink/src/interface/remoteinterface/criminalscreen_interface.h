
#ifndef included_criminalscreeninterface_h
#define included_criminalscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

//! RemoteInterfaceScreen for global criminal database "View Criminal Records" screen
class CriminalScreenInterface : public RemoteInterfaceScreen
{

protected:

    /**
     * The name that's being searched for
     */
	string searchname;

	/**
	 * The index of the criminal in the record
	 */
	int recordindex;

	/**
	 * The time when the page was last re-rendered
	 */
	int lastupdate;

protected:

    /**
     * Updates the screen to display the current recordindex
     */
	void UpdateScreen ();

	/**
	 * Renders the name item
	 * @param button Button for name
	 * @param highlighted Callback for highlight
	 * @param clicked Callback for click
	 */
	static void NameDraw        ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders the name item buorder
	 * @param button Button for name
	 * @param highlighted Callback for highlight
	 * @param clicked Callback for click
	 */
    static void NameBorderDraw  ( Button *button, bool highlighted, bool clicked );

    /**
     * Renders the criminial history
     * @param button
     * @param highlighted Callback for highlight
     * @param clicked Callback for click
     */
    static void HistoryDraw     ( Button *button, bool highlighted, bool clicked );

    /**
     * Closes the current screen
     * @param button
     * @note button is unused
     */
	static void CloseClick			( Button *button );

    /**
     * Adds new conviction user input to the record
     * @param button
     * @note button is unused
     */
	static void AddConvictionClick	( Button *button );

	/**
	 * Clears all convictions from Record
	 * @param button
	 * @note button is unused
	 */
	static void ClearClick			( Button *button );

	/**
	 * Adds an ArrestEvent to the queue
	 * @param button
	 * @note button is unused
	 */
	static void ArrestClick			( Button *button );

public:

	CriminalScreenInterface ();
	~CriminalScreenInterface () override;

    bool EscapeKeyPressed () override;

    /**
     * Sets the name we're currently searching for
     * @param newsearchname New name to search for
     */
	void SetSearchName ( char *newsearchname );

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};



#endif

