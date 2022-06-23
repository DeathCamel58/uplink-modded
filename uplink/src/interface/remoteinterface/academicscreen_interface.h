
#ifndef included_academicscreeninterface_h
#define included_academicscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

//! RemoteInterfaceScreen for the international academic database's "View Record" screen
class AcademicScreenInterface : public RemoteInterfaceScreen
{

protected:

    /**
     * The name that's being searched for
     */
	string searchname;

    /**
     * The index of the person in the record
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
	 * Renders the details
	 * @param button Button to render
	 * @param highlighted Callback for highlight
	 * @param clicked
	 * @note clicked is unused
	 */
	static void DetailsDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Updates the academic Record
	 * @param button
	 * @note button is unused
	 */
	static void CommitClick			( Button *button );

    /**
     * Closes the current screen
     * @param button
     * @note button is unused
     */
	static void CloseClick			( Button *button );

public:

	AcademicScreenInterface ();
	~AcademicScreenInterface () override;

    bool EscapeKeyPressed () override;

    /**
     * Sets the name we're currently searching for
     * @param newsearchname New name to search for
     */
	void SetSearchName (const string &newsearchname );

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};



#endif

