
#ifndef included_proxybypass_h
#define included_proxybypass_h

#include "world/vlocation.h"

#include "interface/taskmanager/uplinktask.h"

//! Security Bypass Class
/**
 * Security Bypass
 * Similar to disable, but this software runs continuously, bypassing a single proxy server
 * @note This is a generic task designed to run on any security system
 */
class SecurityBypass : public UplinkTask
{

protected:

    /**
     * Toggles whether the bypass tool is paused
     * @param button Pause button
     */
	static void PauseClick ( Button *button );

    /**
     * Closes the program
     * @param button
     * @note button is unused
     */
	static void CloseClick ( Button *button );

	/**
	 * Selects the program
	 * @param button Button of program
	 */
	static void BypassClick ( Button *button );

	/**
	 * Renders the status light on the program
	 * @param button Button of program
	 * @param highlighted
	 * @param clicked
	 * @note highlighted and clicked are unused
	 */
	static void StatusLightDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders the program
	 * @param button Button of program
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void ImageButtonDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Should this program be rendered?
	 * @param pid pid of process to check
	 * @return true if it should be
	 * @note If the program task isn't found, this will return false
	 */
	static bool ShouldDraw ( int pid );

protected:

    /**
     * Target IP
     */
	char ip [SIZE_VLOCATION_IP];

public:

    /**
     * Security system type
     */
	int TYPE;

	/**
	 * The status of the current program
	 * @note Possible values are 0 for static, 1 for working, 2 for paused
	 */
	int status;

public:

	SecurityBypass ();
	SecurityBypass ( int newTYPE );
	~SecurityBypass () override;

	/**
	 * Sets the type of security system
	 * @param newTYPE Type of security system
	 */
	void SetTYPE ( int newTYPE );

	/**
	 * Pauses the current bypass
	 */
	void PauseCurrentBypass ();

	/**
	 * Resumes the current bypass
	 */
	void ResumeCurrentBypass ();

	/**
	 * Ends the current bypass
	 * @note This does not kill the program, just disconnects it from what it's working on
	 */
	void EndCurrentBypass ();

	void SetTarget (UplinkObject *uo, const string &uos, int uoi ) override;
	void MoveTo ( int x, int y, int time_ms ) override;

	void Initialise () override;
	void Tick ( int n ) override;
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;


};


#endif


