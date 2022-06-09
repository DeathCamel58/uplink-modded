
#ifndef included_localinterface_h
#define included_localinterface_h


#include "app/uplinkobject.h"


class LocalInterfaceScreen;
class HUDInterface;


// === List all screen codes here =============================================

#define  SCREEN_NONE		0
#define  SCREEN_HUD			1
#define  SCREEN_HW			3
#define  SCREEN_MAP			4
#define  SCREEN_MEMORY		6
#define  SCREEN_STATUS		7
#define  SCREEN_EMAIL		8
#define  SCREEN_FINANCE		9
#define  SCREEN_MISSION		10
#define  SCREEN_SENDMAIL	11
#define  SCREEN_CHEATS		12
#define  SCREEN_EVTQUEUE	13
#define  SCREEN_ANALYSER	14
#define	 SCREEN_GATEWAY     15
#define  SCREEN_IRC			16
#define  SCREEN_LAN         17

// ============================================================================


#define  PANELSIZE		 0.29						// Percentage of screen taken up by right panel


//! Local Interface Class
/**
 * Local Interface class object
 * @note Part of the interface subsystem
 * @note Handles the interface projected by our local machine gateway (for example, the current date/time, worldmap, etc)
 */
class LocalInterface : public UplinkObject  
{

protected:

	LocalInterfaceScreen *screen;	
	HUDInterface *hud;

public:

    /**
     * The ID code of the current screen
     */
	int currentscreencode;
	/**
	 * Used for when SCREENCODE=SCREEN_EMAIL (index of email viewed)
	 */
	int screenindex;

public:

	LocalInterface();
	~LocalInterface() override;

	/**
	 * Close any open screens for current object
	 */
	void Reset ();

	/**
	 * Opens the screen for current object
	 */
	void Create ();

	/**
	 * Close any open screens for current object
	 */
	void Remove ();

	/**
	 * Is there a currently open screen?
	 * @return true if screen visible
	 */
	bool IsVisible ();


	/**
	 * Gets the currently open screen
	 * @return The ID code of the current screen
	 */
	int InScreen ();

	/**
	 * Changes the current screen
	 * @param SCREENCODE The ID code of screen to change to
	 * @param index Index of email viewed
	 * @note index is only used on email screen
	 */
	void RunScreen ( int SCREENCODE, int index = -1 );

	/**
	 * Check if screen ID code is valid
	 * @param SCREENCODE The ID code of screen to check
	 * @param index
	 * @return true if valid screen
	 * @note index is unused
	 */
	static bool VerifyScreen ( int SCREENCODE, int index = -1 );

	/**
	 * Asserts screen
	 * @return The current LocalInterfaceScreen pointer
	 */
	LocalInterfaceScreen *GetInterfaceScreen ();

	/**
	 * Asserts HUD
	 * @return The current HUDInterface pointer
	 */
	HUDInterface *GetHUD ();


	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;

	string GetID () override;

};

#endif 
